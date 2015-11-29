#include <ConfigurationSet.h>
#include <Module.h>
#include <Types.h>
#include <Utils.h>
#include <logging.h>

#include <LHAPDF/LHAPDF.h>
#include <cpp_pp_ttx_fullylept.h>

class MatrixElement: public Module {
    struct ParticleId {
        int64_t pdg_id;
        int64_t me_index;
        int64_t particle_index;
    };

    public:

        MatrixElement(const ConfigurationSet& parameters): Module(parameters.getModuleName()) {
            m_partons = get<std::vector<std::vector<LorentzVector>>>(parameters.get<InputTag>("initialState"));

            const auto& invisibles_set = parameters.get<ConfigurationSet>("invisibles");

            InputTag invisibles_tag = invisibles_set.get<InputTag>("input");
            LOG(debug) << "[MatrixElement] invisibles input tag: " << invisibles_tag.toString();
            m_invisibles = get<std::vector<std::vector<LorentzVector>>>(invisibles_tag);

            const auto& invisibles_jacobians_tag = invisibles_set.get<InputTag>("jacobians");
            LOG(debug) << "[MatrixElement] invisibles jacobians tag: " << invisibles_jacobians_tag.toString();
            m_invisibles_jacobians = get<std::vector<double>>(invisibles_jacobians_tag);

            const auto& invisibles_ids_set = invisibles_set.get<std::vector<ConfigurationSet>>("ids");
            LOG(debug) << "[MatrixElement] # invisibles ids: " << invisibles_ids_set.size();
            for (const auto& s: invisibles_ids_set) {
                ParticleId id;
                id.pdg_id = s.get<int64_t>("pdg_id");
                id.me_index = s.get<int64_t>("me_index");
                id.particle_index = s.get<int64_t>("index");
                m_invisibles_ids.push_back(id);
            }

            const auto& particles_set = parameters.get<ConfigurationSet>("particles");

            InputTag particles_tag = particles_set.get<InputTag>("input");
            LOG(debug) << "[MatrixElement] particles input tag: " << particles_tag.toString();
            m_particles = get<std::vector<LorentzVector>>(particles_tag);

            const auto& particles_ids_set = particles_set.get<std::vector<ConfigurationSet>>("ids");
            LOG(debug) << "[MatrixElement] # particles ids: " << particles_ids_set.size();
            for (const auto& s: particles_ids_set) {
                ParticleId id;
                id.pdg_id = s.get<int64_t>("pdg_id");
                id.me_index = s.get<int64_t>("me_index");
                id.particle_index = s.get<int64_t>("index");
                m_particles_ids.push_back(id);
            }

            const auto& jacobians_tags = parameters.get<std::vector<InputTag>>("jacobians");
            for (const auto& tag: jacobians_tags) {
                m_jacobians.push_back(get<double>(tag));
            }

            m_ME_parameters.reset(new Parameters_sm("/home/sbrochet/Recherche/CP3/MEMpp/Modules/ME/Cards/param_card.dat"));
            m_ME.reset(new cpp_pp_ttx_fullylept(*m_ME_parameters));

            // PDF
            std::string pdf = parameters.get<std::string>("pdf");
            m_pdf.reset(LHAPDF::mkPDF(pdf, 0));

        };

        virtual void work() override {
            static std::vector<LorentzVector> empty_vector;

            m_weights->clear();

            uint64_t n_sols = m_invisibles->size();

            if (!m_invisibles_ids.empty() && !n_sols)
                return;

            // Loop over all solutions if there's, otherwise use only particles
            if (!m_invisibles.get() || m_invisibles->empty()) {
                // No invisibles particles. Use only particles for the matrix element
                // computation
                internal_work((*m_partons)[0], empty_vector, 1, *m_particles);
            } else {
                // Loop over all invisibles solutions
                for (size_t i = 0; i < m_invisibles->size(); i++) {
                    internal_work((*m_partons)[i], (*m_invisibles)[i], (*m_invisibles_jacobians)[i], *m_particles);
                }
            }
            
        }

        virtual void internal_work(const std::vector<LorentzVector>& partons, const std::vector<LorentzVector>& invisibles, double invisibles_jacobian, const std::vector<LorentzVector> particles) {
            std::vector<std::pair<int, std::vector<double>>> finalStates;
            std::vector<int64_t> indexing;

            for (size_t i = 0; i < m_invisibles_ids.size(); i++) {
                finalStates.push_back(std::make_pair(m_invisibles_ids[i].pdg_id, toVector(invisibles[m_invisibles_ids[i].particle_index])));
                indexing.push_back(m_invisibles_ids[i].me_index - 1);
            }

            for (size_t i = 0; i < m_particles_ids.size(); i++) {
                finalStates.push_back(std::make_pair(m_particles_ids[i].pdg_id, toVector(particles[m_particles_ids[i].particle_index])));
                indexing.push_back(m_particles_ids[i].me_index - 1);
            }

            // Sort the array taking into account the indexing in the configuration
            std::vector<int64_t> suite(indexing.size());
            for (size_t i = 0; i < indexing.size(); i++)
                suite[i] = i;

            auto permutations = get_permutations(suite, indexing);
            apply_permutations(finalStates, permutations);

            std::vector<std::vector<double>> initialState(partons.size());
            size_t index = 0;
            for (const auto& parton: partons)
                initialState[index++] = toVector(parton);

            auto result = m_ME->sigmaKin(initialState, finalStates);

            // FIXME
            static const double SQRT_S = 13000.;
            double x1 = std::abs(partons[0].Pz() / (SQRT_S / 2.));
            double x2 = std::abs(partons[1].Pz() / (SQRT_S / 2.));

            // Compute flux factor 1/(2*x1*x2*s)
            double phaseSpaceIn = 1. / (2. * x1 * x2 * SQ(SQRT_S));

            // Compute phase space density for observed particles (not concerned by the change of variable)
            // dPhi = |P|^2 sin(theta)/(2*E*(2pi)^3)
            double phaseSpaceOut = 1.;
            for (const auto& p: particles) {
                phaseSpaceOut *= SQ(p.P()) * sin(p.Theta()) / (2.0 * p.E() * CB(2. * M_PI));
            }

            double weight = phaseSpaceIn * phaseSpaceOut * invisibles_jacobian;
            for (const auto& jacobian: m_jacobians) {
                weight *= (*jacobian);
            }

            // PDF
            double final_weight = 0;
            for (const auto& me: result) {
                //FIXME
                static double M_T = 173;
                double pdf1 = m_pdf->xfxQ2(me.first.first, x1, SQ(M_T)) / x1;
                double pdf2 = m_pdf->xfxQ2(me.first.second, x2, SQ(M_T)) / x2;

                final_weight += me.second * pdf1 * pdf2;
            }

            final_weight *= weight;
            m_weights->push_back(final_weight);
        }

    private:
        std::shared_ptr<const std::vector<std::vector<LorentzVector>>> m_partons;

        std::shared_ptr<const std::vector<std::vector<LorentzVector>>> m_invisibles;
        std::shared_ptr<const std::vector<double>> m_invisibles_jacobians;
        std::vector<ParticleId> m_invisibles_ids;

        std::shared_ptr<const std::vector<LorentzVector>> m_particles;
        std::vector<ParticleId> m_particles_ids;

        std::vector<std::shared_ptr<const double>> m_jacobians;

        std::shared_ptr<Parameters_sm> m_ME_parameters;
        std::shared_ptr<cpp_pp_ttx_fullylept> m_ME;

        std::shared_ptr<LHAPDF::PDF> m_pdf;

        std::shared_ptr<std::vector<double>> m_weights = produce<std::vector<double>>("weights");
};
REGISTER_MODULE(MatrixElement);