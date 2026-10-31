# Inventaire exhaustif des classes et types

Catalogue vérifié : **85 composants demandés**, fichiers headers présents. Types additionnels : AccountId (Money.hpp), TransactionStatus (TransactionType.hpp), Sector (Resource.hpp), Personality (Dictator.hpp), RegimeStatus (PoliticalState.hpp), ActionType (PoliticalAction.hpp), GovernmentReport (StatisticsSnapshot.hpp). Leurs déclarations, responsabilités et tests sont compris dans les entrées de leur header propriétaire.

Tous les composants nommés dans le cahier des charges figurent ci-dessous. IMPLEMENTED signifie que la responsabilité V1 indiquée fonctionne, sans prétention scientifique. PARTIAL signale une responsabilité simplifiée; CONTRACT_ONLY un type/interface préparatoire sans moteur propre. Aucun composant demandé n’est NOT_IMPLEMENTED. Les types simples n’exigent pas de `.cpp`. Les blocs de déclaration donnent exactement les données possédées, méthodes, visibilité et dépendances. Les tests de groupe incluent la couverture indirecte, sans prétendre à un test isolé de chaque formule.

## Simulation

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/core/Simulation.hpp`.
- Implémentation : `src/core/Simulation.cpp`
- Responsabilité : Orchestration déterministe, accès rapport, actions validées.
- Dépendances : "World.hpp", "dictatorsim/statistics/StatisticsEngine.hpp", "dictatorsim/decisions/DecisionManager.hpp".
- Tests associés : SimulationIntegrationTest, SimulationClockTest, ConfigurationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class Simulation {
        World world_;
        StatisticsEngine statistics_;
        DecisionManager decisions_;
        public:
        explicit Simulation(SimulationConfig config);
        void step();
        void run(std::uint64_t days);
        const World& world() const;
        GovernmentReport report() const;
        const StatisticsEngine& statistics() const;
        void setDecisionProvider(IDecisionProvider* provider);
        bool act(const PoliticalAction& action);
    };
}
```

## SimulationClock

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/core/SimulationClock.hpp`.
- Implémentation : `src/core/SimulationClock.cpp`
- Responsabilité : Calendrier 360 jours, limites et échéances mensuelles.
- Dépendances : <cstdint>.
- Tests associés : SimulationIntegrationTest, SimulationClockTest, ConfigurationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class SimulationClock {
        std::uint64_t elapsed_=0;
        public:
        void advance();
        std::uint64_t day() const;
        unsigned month() const;
        std::uint64_t year() const;
        bool monthEnd() const;
    };
}
```

## World

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/core/World.hpp`.
- Implémentation : `src/core/World.cpp`
- Responsabilité : Propriété unique et vérification des états du pays.
- Dépendances : "SimulationConfig.hpp", "SimulationClock.hpp", "dictatorsim/population/PopulationManager.hpp", "dictatorsim/economy/Treasury.hpp", "dictatorsim/economy/Company.hpp", "dictatorsim/economy/EconomicState.hpp", "dictatorsim/events/EventLogger.hpp", "dictatorsim/society/SocialState.hpp", "dictatorsim/society/CrimeEvent.hpp", "dictatorsim/politics/Government.hpp", "dictatorsim/military/MilitaryState.hpp", "dictatorsim/disasters/DisasterEvent.hpp".
- Tests associés : SimulationIntegrationTest, SimulationClockTest, ConfigurationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World {
        IdGenerator ids_;
        RandomEngine populationRandom_;
        std::vector<ProfessionConfig> professions_;
        CitizenFactory factory_;
        public:
        const SimulationConfig config;
        PopulationManager population;
        Ledger ledger;
        Treasury treasury;
        std::vector<Company> companies;
        SimulationClock clock;
        RandomEngine economyRandom, socialRandom, militaryRandom, disasterRandom, politicalRandom;
        EconomicState economy;
        SocialState society;
        Government government;
        MilitaryState military;
        EventBus events;
        EventLogger logger;
        std::vector<CrimeEvent> crimes;
        std::vector<DisasterEvent> disasters;
        explicit World(SimulationConfig config);
        World(const World&)=delete;
        World& operator=(const World&)=delete;
        void event(EventType type, std::uint32_t subject, const std::string& message);
        void verify() const;
        const ProfessionConfig& professionConfig(Profession profession) const;
    };
}
```

## RandomEngine

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/core/RandomEngine.hpp`.
- Implémentation : `src/core/RandomEngine.cpp`
- Responsabilité : Tirages reproductibles validés.
- Dépendances : <cstdint>, <random>, <vector>.
- Tests associés : SimulationIntegrationTest, SimulationClockTest, ConfigurationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds{
    class RandomEngine{
        private:
        std::mt19937 engin;
        public:
            // pour force la const de l'objet et eviter RandomeEngin e =42 
            explicit RandomEngine(std::uint32_t seed);
            int randomInt(int min , int max);
            float randomFloat(float min, float max);
            bool probability(double p);
            std::size_t weightedChoice(
                const std::vector<double>& weights
            );

    };
}
```

## SimulationConfig

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/core/SimulationConfig.hpp`.
- Implémentation : `src/core/SimulationConfig.cpp`
- Responsabilité : Paramètres chargés et validation immuable.
- Dépendances : <cstddef>, <array>, <cstdint>, <string>, "dictatorsim/economy/Money.hpp".
- Tests associés : SimulationIntegrationTest, SimulationClockTest, ConfigurationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct SimulationConfig {
        std::size_t population=1000;
        std::uint32_t seed=42;
        std::uint64_t days=365;
        Money moneySupply=1000000000;
        int citizenShare=4000, companyShare=4000, taxRate=1500;
        Money wage=60000, foodPrice=150, livingCost=300, repairCost=15000, medicalCost=10000;
        std::array<std::array<double,5>,3> regionalRisks {
            {
                {
                    {
                        .6,1,.4,1,0
                    }
                }
                , {
                    {
                        .3,0,1,.6,0
                    }
                }
                , {
                    {
                        1,0,.3,.2,.2
                    }
                }
            }
        };
        double productivity=30, fatigue=2, rest=1.8, crimeProbability=.002, detection=.4;
        double breakdownProbability=.0005, sicknessProbability=.0005, disasterProbability=.001;
        double happinessFood=.35, happinessHousing=.2, happinessHealth=.25, happinessRest=.2;
        double protestProbability=.08, rebellionProbability=.02, coupProbability=.02;
        double militaryShare=.1, corruptionProbability=.001;
        int welfareRate=500;
        std::string configDirectory="configs";
        std::string ledgerArchive;
        void load(const std::string& directory);
        void validate() const;
    };
}
```

## SimulationState

- État : **CONTRACT_ONLY**.
- Header : `include/dictatorsim/core/SimulationState.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Contrat de cycle de vie futur, non utilisé par la boucle actuelle.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : compilation du contrat; pas de comportement actif testé.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    enum class SimulationState {
        Running, Finished
    };
}
```

## IdGenerator

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/tools/IdGenerator.hpp`.
- Implémentation : `src/tools/IdGenerator.cpp`
- Responsabilité : Identifiants séquentiels avec contrôle de débordement.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : IdGeneratorTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds{
    class IdGenerator{
        private: 
        std::uint32_t nextId;
        public:
        IdGenerator();
        std::uint32_t generate();
    };
}
```

## Citizen

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/population/Citizen.hpp`.
- Implémentation : `src/population/Citizen.cpp`
- Responsabilité : Identité, compte référencé, besoins, profession et état individuel.
- Dépendances : <cstdint>, "dictatorsim/economy/Money.hpp", "Profession.hpp".
- Tests associés : CitizenTest, CitizenFactoryTest, PopulationManagerTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
class Ledger;

struct Citizen {
    // Identité
private:
    std::uint32_t id;
    const Ledger* ledger_ = nullptr;
public:
    std::uint8_t age;
    Profession profession;
    // Situation économique
    double income;
    void bindAccount(const Ledger& ledger, AccountId id);
    AccountId account = 0;
    int employer = -1;
    Money earned = 0;
    Money pendingCost = 0;
    int pendingService = -1;
    float housing = 100, stress = 0, riskTolerance = 0.5f;
    bool protesting = false;
    // Besoins
    float foodSatisfaction;
    float health;
    float fatigue;

    // État psychologique et politique
    float happiness;
    float governmentTrust;
    float anger;

    public:
    Citizen(
        std::uint32_t id,
        std::uint8_t age,
        Profession profession
        );
    std::uint32_t getId()const;
    std::uint8_t getAge()const;
    Profession getProfession()const;

    double getIncome()const;
    double getSavings()const;


    float getFoodSatisfaction()const;
    float getHealth()const;
    float getFatigue()const;


    float getHappiness()const;
    float getGovernmentTrust() const;
    float getAnger()const;


};

}
```

## Profession

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/population/Profession.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Catalogue de métiers existant.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : CitizenTest, CitizenFactoryTest, PopulationManagerTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds{
    enum class Profession {
        Unemployed,
        Student,
        Retired,
        Farmer,
        Baker,
        Teacher,
        Researcher,
        Engineer,
        Doctor,
        Worker,
        Soldier,
    };
}
```

## ProfessionConfig

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/population/ProfessionConfig.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Paramètres de génération et de métier.
- Dépendances : <cstdint>, "dictatorsim/economy/Resource.hpp", "dictatorsim/economy/Money.hpp", "dictatorsim/population/Profession.hpp".
- Tests associés : CitizenTest, CitizenFactoryTest, PopulationManagerTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {

struct ProfessionConfig {
    Profession profession;

    std::uint8_t minAge;
    std::uint8_t maxAge;

    double weight;
    Money referenceSalary=60000;
    Sector sector=Sector::Commerce;
    double productivity=1, physicalLoad=1, mentalLoad=1, workHours=8, skill=0;

};

}
```

## CitizenFactory

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/population/CitizenFactory.hpp`.
- Implémentation : `src/population/CitizenFactory.cpp`
- Responsabilité : Création des citoyens sans calcul métier.
- Dépendances : <cstddef>, <cstdint>, <vector>, "dictatorsim/population/Citizen.hpp", "dictatorsim/core/RandomEngine.hpp", "dictatorsim/tools/IdGenerator.hpp", "dictatorsim/population/ProfessionConfig.hpp".
- Tests associés : CitizenTest, CitizenFactoryTest, PopulationManagerTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds{
    class CitizenFactory{
        private : 
            IdGenerator& idGenerator;
            RandomEngine& randomEngine;
            std::vector<ProfessionConfig> professions;


            std::uint8_t generateAge();
            Profession generateProfession(std::uint8_t age);
        public:
        CitizenFactory(
            IdGenerator&idGenerator,
            RandomEngine& randomEngine,
            const std::vector<ProfessionConfig>& professions
        );
        Citizen createCitizen();
        std::vector<Citizen> createPopulation(std::size_t size);

    };
}
```

## PopulationManager

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/population/PopulationManager.hpp`.
- Implémentation : `src/population/PopulationManager.cpp`
- Responsabilité : Propriété du vecteur et index id→position.
- Dépendances : <cstddef>, <cstdint>, <unordered_map>, <vector>, "dictatorsim/population/Citizen.hpp", "dictatorsim/population/CitizenFactory.hpp".
- Tests associés : CitizenTest, CitizenFactoryTest, PopulationManagerTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {

class PopulationManager {
private:
    std::vector<Citizen> population;

    // Citizen ID -> position in vector
    std::unordered_map<std::uint32_t, std::size_t> citizenIndex;

    CitizenFactory& factory;

public:
    explicit PopulationManager(CitizenFactory& factory);

    // Population generation
    void generatePopulation(std::size_t size);

    // Population access
    const std::vector<Citizen>& getPopulation() const;

    std::size_t getPopulationSize() const;

    // Citizen lookup
    const Citizen* findCitizen(std::uint32_t id) const;
    Citizen* findCitizen(std::uint32_t id);

    // Citizen deletion
    bool deleteCitizen(std::uint32_t id);
};

} // namespace ds
```

## IProfessionProvider

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/providers/IProfessionProvider.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Contrat de chargement des professions.
- Dépendances : <vector>, "dictatorsim/population/ProfessionConfig.hpp".
- Tests associés : CSVProfessionProviderTest, ConfigurationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {

class IProfessionProvider {
public:
    virtual ~IProfessionProvider() = default;

    virtual std::vector<ProfessionConfig> load() const = 0;
};

}
```

## CSVProfessionProvider

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/providers/CSVProfessionProvider.hpp`.
- Implémentation : `src/providers/CSVProfessionProvider.cpp`
- Responsabilité : Chargement et fusion des CSV de professions.
- Dépendances : <string>, "dictatorsim/providers/IProfessionProvider.hpp".
- Tests associés : CSVProfessionProviderTest, ConfigurationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {

class CSVProfessionProvider : public IProfessionProvider {
private:
    std::string filePath;

public:
    explicit CSVProfessionProvider(const std::string& filePath);

    std::vector<ProfessionConfig> load() const override;
    std::vector<ProfessionConfig> loadWithEconomics(const std::string& economicPath) const;
};

}
```

## Event

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/events/Event.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Fait ou message explicatif daté.
- Dépendances : "EventType.hpp", <cstdint>, <string>.
- Tests associés : SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct Event {
        EventType type;
        std::uint64_t day;
        std::uint32_t subject;
        std::string message;
    };
}
```

## EventBus

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/events/EventBus.hpp`.
- Implémentation : `src/events/EventBus.cpp`
- Responsabilité : File FIFO sans callbacks récursifs.
- Dépendances : "Event.hpp", <vector>.
- Tests associés : SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class EventBus {
        std::vector<Event> pending_;
        public:
        void publish(Event event);
        std::vector<Event> drain();
    };
}
```

## EventType

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/events/EventType.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Catalogue des faits.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    enum class EventType {
        CitizenBecameUnemployed, CitizenBecameSick, CarBreakdown, EarthquakeOccurred, TsunamiOccurred, DroughtStarted, FloodOccurred, VolcanicEruptionOccurred, CrimeOccurred, ProtestStarted, RebellionStarted, MilitaryCoupStarted, GovernmentCollapsed, PaymentRefused, PaymentCompleted, PolicyApplied, PricesChanged
    };
}
```

## EventLogger

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/events/EventLogger.hpp`.
- Implémentation : `src/events/EventLogger.cpp`
- Responsabilité : Collecte et sortie lisible.
- Dépendances : "EventBus.hpp", <ostream>.
- Tests associés : SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class EventLogger {
        std::vector<Event> events_;
        public:
        void collect(EventBus& bus);
        const std::vector<Event>& entries() const;
        void print(std::ostream& out, std::size_t last=20) const;
    };
}
```

## Money

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/economy/Money.hpp`.
- Implémentation : `src/economy/Money.cpp`
- Responsabilité : Montant entier en centimes et fraction à arrondi explicite.
- Dépendances : <cstdint>.
- Tests associés : LedgerTest, AccountTest, CompanyTest, EconomyEngineTest, ArchiveTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    using Money = std::int64_t;
    using AccountId = std::uint32_t;
    // Basis points, rounded down; avoids intermediate multiplication overflow.
    Money fraction(Money amount, int basisPoints);
}
```

## Account

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/economy/Account.hpp`.
- Implémentation : `src/economy/Account.cpp`
- Responsabilité : Solde privé contrôlé par Ledger.
- Dépendances : "Money.hpp", <string>.
- Tests associés : LedgerTest, AccountTest, CompanyTest, EconomyEngineTest, ArchiveTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class Ledger;
    class Account {
        friend class Ledger;
        AccountId id_;
        Money balance_;
        std::string owner_;
        public:
        Account(AccountId id, Money balance, std::string owner);
        AccountId id() const;
        Money balance() const;
        const std::string& owner() const;
    };
}
```

## Transaction

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/economy/Transaction.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Fait comptable daté, avec statut.
- Dépendances : "Money.hpp", "TransactionType.hpp".
- Tests associés : LedgerTest, AccountTest, CompanyTest, EconomyEngineTest, ArchiveTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct Transaction {
        std::uint64_t id, day;
        AccountId source, destination;
        Money amount;
        TransactionType type;
        TransactionStatus status;
    };
}
```

## TransactionType

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/economy/TransactionType.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Nature des flux et motif des refus.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : LedgerTest, AccountTest, CompanyTest, EconomyEngineTest, ArchiveTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    enum class TransactionType {
        Wage, Purchase, Tax, Repair, Medical, Reconstruction, Theft, Restitution, Fine, Subsidy, Contract, Corruption, MonetaryCreation, MonetaryDestruction
    };
    enum class TransactionStatus {
        Accepted, InvalidAmount, UnknownAccount, SameAccount, InsufficientFunds, Overflow
    };
}
```

## Ledger

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/economy/Ledger.hpp`.
- Implémentation : `src/economy/Ledger.cpp`
- Responsabilité : Comptes, transferts atomiques, conservation et archive.
- Dépendances : "Account.hpp", "Transaction.hpp", <vector>, <fstream>.
- Tests associés : LedgerTest, AccountTest, CompanyTest, EconomyEngineTest, ArchiveTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    // Owns every monetary balance. No business system can mint money.
    class Ledger {
        std::vector<Account> accounts_;
        std::vector<Transaction> journal_;
        Money initial_ = 0;
        bool sealed_ = false;
        std::ofstream archive_;
        std::uint64_t nextTransaction_ = 0;
        // Reserved for a future audited monetary authority; deliberately inaccessible.
        void monetaryOperation(Money amount);
        public:
        AccountId openAccount(Money initial, const std::string& owner);
        void seal();
        void archiveTo(const std::string& path);
        void flushArchive();
        bool transfer(AccountId from, AccountId to, Money amount, TransactionType type, std::uint64_t day);
        Money balance(AccountId id) const;
        Money moneySupply() const;
        Money initialSupply() const;
        bool conserved() const;
        const std::vector<Transaction>& transactions() const;
        const std::vector<Account>& accounts() const;
    };
}
```

## Treasury

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/economy/Treasury.hpp`.
- Implémentation : `src/economy/Treasury.cpp`
- Responsabilité : Façade du compte public.
- Dépendances : "Ledger.hpp".
- Tests associés : LedgerTest, AccountTest, CompanyTest, EconomyEngineTest, ArchiveTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class Treasury {
        AccountId account_;
        public:
        explicit Treasury(AccountId account);
        AccountId account() const;
        Money balance(const Ledger& ledger) const;
        bool pay(Ledger& ledger, AccountId to, Money amount, TransactionType type, std::uint64_t day) const;
    };
}
```

## EconomicState

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/economy/EconomicState.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Agrégats économiques distincts des comptes.
- Dépendances : "Money.hpp", <cstddef>.
- Tests associés : LedgerTest, AccountTest, CompanyTest, EconomyEngineTest, ArchiveTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct EconomicState {
        double totalProduction=0, priceIndex=1, inflation=0, prosperity=0;
        std::size_t purchases=0, refused=0;
        Money wagesPaid=0;
        double agriculturalMultiplier=1;
        std::uint64_t droughtUntil=0;
    };
}
```

## EconomyEngine

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/economy/EconomyEngine.hpp`.
- Implémentation : `src/economy/EconomyEngine.cpp`
- Responsabilité : Ordonnancement des neuf systèmes économiques.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : LedgerTest, AccountTest, CompanyTest, EconomyEngineTest, ArchiveTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class EconomyEngine {
        public: void update(World& world) const;
    };
}
```

## Company

- État : **PARTIAL**.
- Header : `include/dictatorsim/economy/Company.hpp`.
- Implémentation : `src/economy/Company.cpp`
- Responsabilité : Employés, capacité, production physique, vente et dégâts.
- Dépendances : "Resource.hpp", "Ledger.hpp", <vector>.
- Tests associés : LedgerTest, AccountTest, CompanyTest, EconomyEngineTest, ArchiveTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class Company {
        public:
        std::uint32_t id;
        Sector sector;
        AccountId account;
        std::vector<std::uint32_t> employees;
        double productivity, capacity, stock=0;
        Money price;
        Money revenue=0;
        Company(std::uint32_t id, Sector sector, AccountId account, double productivity, Money price);
        double produce(double labor, double multiplier);
        bool sell(Ledger& ledger, AccountId buyer, std::uint64_t day, TransactionType type=TransactionType::Purchase);
        void damage(double fraction);
    };
}
```

## Resource

- État : **PARTIAL**.
- Header : `include/dictatorsim/economy/Resource.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Nomenclature sectorielle et contrat futur de ressources multiples.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : LedgerTest, AccountTest, CompanyTest, EconomyEngineTest, ArchiveTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    enum class Sector {
        Agriculture, OilIndustry, Manufacturing, Construction, Healthcare, Commerce
    };
    enum class Resource {
        Food, Oil, Goods, BuildingMaterial, Treatment, Service
    };
}
```

## Market

- État : **PARTIAL**.
- Header : `include/dictatorsim/economy/Market.hpp`.
- Implémentation : `src/economy/Market.cpp`
- Responsabilité : Recherche déterministe du fournisseur sectoriel; moteurs utilisent aussi leurs indices connus.
- Dépendances : "Company.hpp".
- Tests associés : LedgerTest, AccountTest, CompanyTest, EconomyEngineTest, ArchiveTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class Market {
        public: static Company* seller(std::vector<Company>& companies, Sector sector);
    };
}
```

## IncomeSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/economy/systems/IncomeSystem.hpp`.
- Implémentation : `src/economy/systems/IncomeSystem.cpp`
- Responsabilité : Allocations publiques mensuelles financées.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : IncomeSystemTest, ConsumptionSystemTest, TaxationSystemTest, EconomyEngineTest, ExceptionalPaymentTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class IncomeSystem {
        public: void update(World& world) const;
    };
}
```

## WageSystem

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/economy/systems/WageSystem.hpp`.
- Implémentation : `src/economy/systems/WageSystem.cpp`
- Responsabilité : Salaires privés mensuels, refus sans liquidités.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : IncomeSystemTest, ConsumptionSystemTest, TaxationSystemTest, EconomyEngineTest, ExceptionalPaymentTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class WageSystem {
        public: void update(World& world) const;
    };
}
```

## ConsumptionSystem

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/economy/systems/ConsumptionSystem.hpp`.
- Implémentation : `src/economy/systems/ConsumptionSystem.cpp`
- Responsabilité : Achats alimentaires contre stock et paiement.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : IncomeSystemTest, ConsumptionSystemTest, TaxationSystemTest, EconomyEngineTest, ExceptionalPaymentTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class ConsumptionSystem {
        public: void update(World& world) const;
    };
}
```

## LivingCostSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/economy/systems/LivingCostSystem.hpp`.
- Implémentation : `src/economy/systems/LivingCostSystem.cpp`
- Responsabilité : Besoins ordinaires, pannes, soins, factures en attente.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : IncomeSystemTest, ConsumptionSystemTest, TaxationSystemTest, EconomyEngineTest, ExceptionalPaymentTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class LivingCostSystem {
        public: void update(World& world) const;
    };
}
```

## TaxationSystem

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/economy/systems/TaxationSystem.hpp`.
- Implémentation : `src/economy/systems/TaxationSystem.cpp`
- Responsabilité : Taxes sur salaires réellement versés.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : IncomeSystemTest, ConsumptionSystemTest, TaxationSystemTest, EconomyEngineTest, ExceptionalPaymentTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class TaxationSystem {
        public: void update(World& world) const;
    };
}
```

## ProductionSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/economy/systems/ProductionSystem.hpp`.
- Implémentation : `src/economy/systems/ProductionSystem.cpp`
- Responsabilité : Production selon travail, santé, capacité et climat.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : IncomeSystemTest, ConsumptionSystemTest, TaxationSystemTest, EconomyEngineTest, ExceptionalPaymentTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class ProductionSystem {
        public: void update(World& world) const;
    };
}
```

## PricingSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/economy/systems/PricingSystem.hpp`.
- Implémentation : `src/economy/systems/PricingSystem.cpp`
- Responsabilité : Ajustement mensuel selon stocks.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : IncomeSystemTest, ConsumptionSystemTest, TaxationSystemTest, EconomyEngineTest, ExceptionalPaymentTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class PricingSystem {
        public: void update(World& world) const;
    };
}
```

## InflationSystem

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/economy/systems/InflationSystem.hpp`.
- Implémentation : `src/economy/systems/InflationSystem.cpp`
- Responsabilité : Indice d’un panier fixe et variation relative.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : IncomeSystemTest, ConsumptionSystemTest, TaxationSystemTest, EconomyEngineTest, ExceptionalPaymentTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class InflationSystem {
        public: void update(World& world) const;
    };
}
```

## TradeSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/economy/systems/TradeSystem.hpp`.
- Implémentation : `src/economy/systems/TradeSystem.cpp`
- Responsabilité : Ventes intérieures d’intrants et restauration simplifiée de capacité.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : IncomeSystemTest, ConsumptionSystemTest, TaxationSystemTest, EconomyEngineTest, ExceptionalPaymentTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class TradeSystem {
        public: void update(World& world) const;
    };
}
```

## SocietyEngine

- État : **PARTIAL**.
- Header : `include/dictatorsim/society/SocietyEngine.hpp`.
- Implémentation : `src/society/SocietyEngine.cpp`
- Responsabilité : Ordre des mécanismes sociaux.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : HappinessSystemTest, CrimeSystemTest, ProtestSystemTest, RebellionSystemTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class SocietyEngine {
        public: void update(World& world) const;
    };
}
```

## SocialState

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/society/SocialState.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Agrégats et mobilisation.
- Dépendances : <cstddef>.
- Tests associés : HappinessSystemTest, CrimeSystemTest, ProtestSystemTest, RebellionSystemTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct SocialState {
        double happiness=50, anger=0, health=100, inequality=0, wellbeing=50;
        std::size_t protesters=0, actualCrimes=0, reportedCrimes=0, attempts=0;
        bool rebellion=false;
    };
}
```

## SocialGroup

- État : **CONTRACT_ONLY**.
- Header : `include/dictatorsim/society/SocialGroup.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Contrat futur de groupes géographiques.
- Dépendances : <cstdint>, <vector>.
- Tests associés : compilation du contrat; pas de comportement actif testé.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct SocialGroup {
        std::uint32_t region;
        std::vector<std::uint32_t> members;
    };
}
```

## CrimeProfile

- État : **CONTRACT_ONLY**.
- Header : `include/dictatorsim/society/CrimeProfile.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Contrat futur de facteurs criminels; calculs actuels locaux.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : compilation du contrat; pas de comportement actif testé.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct CrimeProfile {
        double desperation, opportunity, detectionRisk;
    };
}
```

## CrimeType

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/society/CrimeType.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Catalogue des infractions V1.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : HappinessSystemTest, CrimeSystemTest, ProtestSystemTest, RebellionSystemTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    enum class CrimeType {
        Theft
    };
}
```

## CrimeEvent

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/society/CrimeEvent.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Trace tentative/succès/signalement/restitution/sanction.
- Dépendances : <cstdint>, "CrimeType.hpp".
- Tests associés : HappinessSystemTest, CrimeSystemTest, ProtestSystemTest, RebellionSystemTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct CrimeEvent {
        std::uint32_t author, victim;
        bool success, reported, restituted, sanctioned;
    };
}
```

## HappinessSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/society/systems/HappinessSystem.hpp`.
- Implémentation : `src/society/systems/HappinessSystem.cpp`
- Responsabilité : Besoins, repos, santé, bonheur et opinions distinctes.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : HappinessSystemTest, CrimeSystemTest, ProtestSystemTest, RebellionSystemTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class HappinessSystem {
        public: void update(World& world) const;
    };
}
```

## SocialInfluenceSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/society/systems/SocialInfluenceSystem.hpp`.
- Implémentation : `src/society/systems/SocialInfluenceSystem.cpp`
- Responsabilité : Diffusion de confiance sur anneau via instantané.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : HappinessSystemTest, CrimeSystemTest, ProtestSystemTest, RebellionSystemTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class SocialInfluenceSystem {
        public: void update(World& world) const;
    };
}
```

## ProtestSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/society/systems/ProtestSystem.hpp`.
- Implémentation : `src/society/systems/ProtestSystem.cpp`
- Responsabilité : Participation selon motifs et risque.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : HappinessSystemTest, CrimeSystemTest, ProtestSystemTest, RebellionSystemTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class ProtestSystem {
        public: void update(World& world) const;
    };
}
```

## RebellionSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/society/systems/RebellionSystem.hpp`.
- Implémentation : `src/society/systems/RebellionSystem.cpp`
- Responsabilité : Mobilisation probabiliste et issues de rébellion.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : HappinessSystemTest, CrimeSystemTest, ProtestSystemTest, RebellionSystemTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class RebellionSystem {
        public: void update(World& world) const;
    };
}
```

## CrimeSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/society/systems/CrimeSystem.hpp`.
- Implémentation : `src/society/systems/CrimeSystem.cpp`
- Responsabilité : Tentatives, vol comptable, restitution et amende.
- Dépendances : "dictatorsim/society/CrimeEvent.hpp".
- Tests associés : HappinessSystemTest, CrimeSystemTest, ProtestSystemTest, RebellionSystemTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class CrimeSystem {
        public:
        void update(World& world) const;
        CrimeEvent attempt(World& world, std::uint32_t author, std::uint32_t victim, bool succeeds, bool reported) const;
    };
}
```

## InequalitySystem

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/society/systems/InequalitySystem.hpp`.
- Implémentation : `src/society/systems/InequalitySystem.cpp`
- Responsabilité : Coefficient de Gini sur soldes citoyens.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : HappinessSystemTest, CrimeSystemTest, ProtestSystemTest, RebellionSystemTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class InequalitySystem {
        public: void update(World& world) const;
    };
}
```

## Government

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/politics/Government.hpp`.
- Implémentation : `src/politics/Government.cpp`
- Responsabilité : Composition politique, état, dirigeant.
- Dépendances : "Policy.hpp", "PoliticalState.hpp", "Dictator.hpp".
- Tests associés : PolicyExecutorTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class Government {
        public: Policy policy;
        PoliticalState state;
        Dictator leader;
        explicit Government(int tax, int welfare, double police);
    };
}
```

## Dictator

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/politics/Dictator.hpp`.
- Implémentation : `src/politics/Dictator.cpp`
- Responsabilité : Personnalité configurée et validée.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : PolicyExecutorTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct Personality {
        double ego=.5, paranoia=.5, greed=.5, intelligence=.5, empathy=.5, authoritarianism=.5, riskTolerance=.5, ambition=.5;
    };
    class Dictator {
        Personality personality_;
        public: explicit Dictator(Personality p={});
        const Personality& personality() const;
    };
}
```

## PoliticalState

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/politics/PoliticalState.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Légitimité, stabilité et statut du régime.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : PolicyExecutorTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    enum class RegimeStatus {
        Governing, Transition, Collapsed, Crisis
    };
    struct PoliticalState {
        double legitimacy=50, stability=70;
        RegimeStatus regime=RegimeStatus::Governing;
        unsigned leadershipChanges=0;
    };
}
```

## Policy

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/politics/Policy.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Paramètres appliqués par les moteurs.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : PolicyExecutorTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct Policy {
        int incomeTax=1500;
        int welfareRate=500;
        double police=.4;
        bool emergency=false;
    };
}
```

## PoliticalAction

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/politics/PoliticalAction.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Action structurée sans accès au monde.
- Dépendances : "dictatorsim/economy/Money.hpp".
- Tests associés : PolicyExecutorTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    enum class ActionType {
        IncreaseTaxes, DecreaseTaxes, IncreaseMilitaryBudget, IncreaseEducationBudget, IncreaseHealthcareBudget, SubsidizeFood, BuildHospital, InvestInIndustry, DeclareEmergency, IncreasePoliceBudget, ProvideDisasterRelief
    };
    struct PoliticalAction {
        ActionType type;
        Money amount=0;
        int basisPoints=0;
    };
}
```

## PolicyExecutor

- État : **PARTIAL**.
- Header : `include/dictatorsim/politics/PolicyExecutor.hpp`.
- Implémentation : `src/politics/PolicyExecutor.cpp`
- Responsabilité : Validation puis effets budgétaires et physiques.
- Dépendances : "PoliticalAction.hpp".
- Tests associés : PolicyExecutorTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class PolicyExecutor {
        public: bool execute(World& world,const PoliticalAction& action) const;
    };
}
```

## ActionValidator

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/politics/ActionValidator.hpp`.
- Implémentation : `src/politics/ActionValidator.cpp`
- Responsabilité : Contrôle faisabilité avant effet.
- Dépendances : "PoliticalAction.hpp", <string>.
- Tests associés : PolicyExecutorTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class ActionValidator {
        public: std::string validate(const World& world,const PoliticalAction& action) const;
    };
}
```

## LegitimacySystem

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/politics/systems/LegitimacySystem.hpp`.
- Implémentation : `src/politics/systems/LegitimacySystem.cpp`
- Responsabilité : Agrégation de la confiance.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : PolicyExecutorTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class LegitimacySystem {
        public: void update(World& world) const;
    };
}
```

## StabilitySystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/politics/systems/StabilitySystem.hpp`.
- Implémentation : `src/politics/systems/StabilitySystem.cpp`
- Responsabilité : Indice politique multi-facteurs.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : PolicyExecutorTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class StabilitySystem {
        public: void update(World& world) const;
    };
}
```

## CorruptionSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/politics/systems/CorruptionSystem.hpp`.
- Implémentation : `src/politics/systems/CorruptionSystem.cpp`
- Responsabilité : Détournement vers un compte existant.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : PolicyExecutorTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class CorruptionSystem {
        public: void update(World& world) const;
    };
}
```

## MilitaryEngine

- État : **PARTIAL**.
- Header : `include/dictatorsim/military/MilitaryEngine.hpp`.
- Implémentation : `src/military/MilitaryEngine.cpp`
- Responsabilité : Coordination militaire.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : MilitaryEngineTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class MilitaryEngine {
        public: void update(World& world) const;
    };
}
```

## MilitaryState

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/military/MilitaryState.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Budget limite, dépenses, rémunération et loyauté.
- Dépendances : "MilitaryUnit.hpp", "dictatorsim/economy/Money.hpp".
- Tests associés : MilitaryEngineTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct MilitaryState {
        MilitaryUnit unit;
        Money monthlyBudget=0, spent=0;
        std::size_t paid=0;
        double loyalty=80, satisfaction=80;
    };
}
```

## MilitaryUnit

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/military/MilitaryUnit.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Références aux citoyens soldats.
- Dépendances : <vector>, <cstdint>.
- Tests associés : MilitaryEngineTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct MilitaryUnit {
        std::vector<std::uint32_t> citizens;
    };
}
```

## LoyaltySystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/military/systems/LoyaltySystem.hpp`.
- Implémentation : `src/military/systems/LoyaltySystem.cpp`
- Responsabilité : Soldes financées et satisfaction/loyauté.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : MilitaryEngineTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class LoyaltySystem {
        public: void update(World& world) const;
    };
}
```

## RecruitmentSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/military/systems/RecruitmentSystem.hpp`.
- Implémentation : `src/military/systems/RecruitmentSystem.cpp`
- Responsabilité : Réconciliation des effectifs existants; pas de recrutement actif.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : MilitaryEngineTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class RecruitmentSystem {
        public: void update(World& world) const;
    };
}
```

## CoupSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/military/systems/CoupSystem.hpp`.
- Implémentation : `src/military/systems/CoupSystem.cpp`
- Responsabilité : Tentatives probabilistes et trois issues.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : MilitaryEngineTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class CoupSystem {
        public: void update(World& world) const;
    };
}
```

## DisasterSystem

- État : **PARTIAL**.
- Header : `include/dictatorsim/disasters/DisasterSystem.hpp`.
- Implémentation : `src/disasters/DisasterSystem.cpp`
- Responsabilité : Tirages régionaux et effets physiques.
- Dépendances : "DisasterEvent.hpp".
- Tests associés : DisasterSystemTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class DisasterSystem {
        public: void update(World& world) const;
        void apply(World& world,const DisasterEvent& event) const;
    };
}
```

## DisasterEvent

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/disasters/DisasterEvent.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Type, région, sévérité et jour.
- Dépendances : "DisasterType.hpp", <cstdint>.
- Tests associés : DisasterSystemTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct DisasterEvent {
        DisasterType type;
        unsigned region;
        double severity;
        std::uint64_t day;
    };
}
```

## DisasterType

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/disasters/DisasterType.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Catalogue des cinq aléas.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : DisasterSystemTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    enum class DisasterType {
        Earthquake, Tsunami, Drought, Flood, VolcanicEruption
    };
}
```

## Earthquake

- État : **CONTRACT_ONLY**.
- Header : `include/dictatorsim/disasters/Earthquake.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Contrat de données séisme.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : compilation du contrat; pas de comportement actif testé.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct Earthquake {
        double relativeRisk=1.0;
    };
}
```

## Tsunami

- État : **CONTRACT_ONLY**.
- Header : `include/dictatorsim/disasters/Tsunami.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Contrat de données tsunami.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : compilation du contrat; pas de comportement actif testé.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct Tsunami {
        double relativeRisk=.2;
    };
}
```

## Drought

- État : **CONTRACT_ONLY**.
- Header : `include/dictatorsim/disasters/Drought.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Contrat de données sécheresse.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : compilation du contrat; pas de comportement actif testé.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct Drought {
        double relativeRisk=1.5;
    };
}
```

## Flood

- État : **CONTRACT_ONLY**.
- Header : `include/dictatorsim/disasters/Flood.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Contrat de données inondation.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : compilation du contrat; pas de comportement actif testé.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct Flood {
        double relativeRisk=.8;
    };
}
```

## VolcanicEruption

- État : **CONTRACT_ONLY**.
- Header : `include/dictatorsim/disasters/VolcanicEruption.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Contrat de données éruption.
- Dépendances : types standards / forward declarations, voir contrat.
- Tests associés : compilation du contrat; pas de comportement actif testé.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct VolcanicEruption {
        double relativeRisk=.1;
    };
}
```

## IDecisionProvider

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/decisions/IDecisionProvider.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Interface non bloquante de propositions structurées.
- Dépendances : "DecisionRequest.hpp", "DecisionResult.hpp", <optional>.
- Tests associés : DecisionManagerTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class IDecisionProvider {
        public: virtual ~IDecisionProvider()=default;
        virtual void request(const DecisionRequest& request)=0;
        virtual std::optional<DecisionResult> poll()=0;
    };
}
```

## DecisionManager

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/decisions/DecisionManager.hpp`.
- Implémentation : `src/decisions/DecisionManager.cpp`
- Responsabilité : Pont rapport/action, filtre des résultats déjà consommés.
- Dépendances : "IDecisionProvider.hpp".
- Tests associés : DecisionManagerTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class DecisionManager {
        IDecisionProvider* provider_=nullptr;
        std::uint64_t requestId_=0;
        std::uint64_t minimumResult_=0;
        public: void setProvider(IDecisionProvider* provider);
        void update(World& world,const GovernmentReport& report);
    };
}
```

## DecisionRequest

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/decisions/DecisionRequest.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Rapport exact et identifiant de requête.
- Dépendances : "dictatorsim/statistics/StatisticsSnapshot.hpp".
- Tests associés : DecisionManagerTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct DecisionRequest {
        std::uint64_t id;
        GovernmentReport report;
    };
}
```

## DecisionResult

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/decisions/DecisionResult.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Action proposée et validation.
- Dépendances : "dictatorsim/politics/PoliticalAction.hpp".
- Tests associés : DecisionManagerTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct DecisionResult {
        std::uint64_t requestId;
        PoliticalAction action;
        bool approved=true;
    };
}
```

## HumanDecisionProvider

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/decisions/HumanDecisionProvider.hpp`.
- Implémentation : `src/decisions/HumanDecisionProvider.cpp`
- Responsabilité : File de commandes humaines sur thread propriétaire.
- Dépendances : "IDecisionProvider.hpp", <deque>.
- Tests associés : DecisionManagerTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    // Single-thread event-loop queue. A graphical controller submits on its owning thread.
    class HumanDecisionProvider: public IDecisionProvider {
        std::deque<DecisionResult> pending_;
        public: void request(const DecisionRequest& request) override;
        void submit(DecisionResult result);
        std::optional<DecisionResult> poll() override;
    };
}
```

## AIDecisionProvider

- État : **CONTRACT_ONLY**.
- Header : `include/dictatorsim/decisions/AIDecisionProvider.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Contrat abstrait futur adaptateur LLM, pas de faux modèle.
- Dépendances : "IDecisionProvider.hpp".
- Tests associés : compilation du contrat; pas de comportement actif testé.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    // Offline structured proposal contract. A future adapter implements request/poll.
    class AIDecisionProvider: public IDecisionProvider {
        public: ~AIDecisionProvider() override=default;
    };
}
```

## HybridDecisionProvider

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/decisions/HybridDecisionProvider.hpp`.
- Implémentation : `src/decisions/HybridDecisionProvider.cpp`
- Responsabilité : Proposition retenue jusqu’à approbation explicite.
- Dépendances : "IDecisionProvider.hpp".
- Tests associés : DecisionManagerTest, SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class HybridDecisionProvider: public IDecisionProvider {
        IDecisionProvider& proposals_;
        std::optional<DecisionResult> awaiting_;
        bool approved_=false;
        public: explicit HybridDecisionProvider(IDecisionProvider& proposals);
        void request(const DecisionRequest& request) override;
        std::optional<DecisionResult> poll() override;
        std::optional<DecisionResult> proposal() const;
        void approve(bool accepted);
    };
}
```

## StatisticsEngine

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/statistics/StatisticsEngine.hpp`.
- Implémentation : `src/statistics/StatisticsEngine.cpp`
- Responsabilité : Calcul quotidien et historique.
- Dépendances : "HistoryManager.hpp".
- Tests associés : SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class StatisticsEngine {
        HistoryManager history_;
        public: void update(World& world);
        const HistoryManager& history() const;
    };
}
```

## StatisticsSnapshot

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/statistics/StatisticsSnapshot.hpp`.
- Implémentation : Aucune (type de données ou contrat).
- Responsabilité : Valeurs observées, sans référence mutable.
- Dépendances : "dictatorsim/economy/Money.hpp", <cstddef>.
- Tests associés : SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    struct StatisticsSnapshot {
        std::uint64_t day=0;
        std::size_t population=0, activeProtesters=0, crimeIncidents=0;
        double averageHappiness=0, averageAnger=0, averageHealth=0, unemploymentRate=0;
        double economicProsperity=0, nationalStability=0, nationalWellbeing=0, militaryLoyalty=0, crimeRate=0;
        Money treasuryBalance=0, moneySupply=0, initialMoney=0;
        double totalProduction=0, priceIndex=1;
    };
    // Exact report today; later may be delayed/noisy without exposing World.
    struct GovernmentReport {
        StatisticsSnapshot observed;
    };
}
```

## HistoryManager

- État : **IMPLEMENTED**.
- Header : `include/dictatorsim/statistics/HistoryManager.hpp`.
- Implémentation : `src/statistics/HistoryManager.cpp`
- Responsabilité : Historique ordonné des snapshots.
- Dépendances : "StatisticsSnapshot.hpp", <vector>.
- Tests associés : SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class HistoryManager {
        std::vector<StatisticsSnapshot> snapshots_;
        public: void append(StatisticsSnapshot snapshot);
        const std::vector<StatisticsSnapshot>& snapshots() const;
    };
}
```

## MetricsCalculator

- État : **PARTIAL**.
- Header : `include/dictatorsim/statistics/MetricsCalculator.hpp`.
- Implémentation : `src/statistics/MetricsCalculator.cpp`
- Responsabilité : Calcul des agrégats et dénominateurs explicites.
- Dépendances : "StatisticsSnapshot.hpp".
- Tests associés : SimulationIntegrationTest.

Contrat exact — données possédées et méthodes publiques (les sections privées sont explicites) :

```cpp
namespace ds {
    class World;
    class MetricsCalculator {
        public: StatisticsSnapshot calculate(const World& world) const;
    };
}
```

## Adaptations de l’existant

Citizen : id privé, suppression du champ savings, getter compatible lisant Ledger, référence de compte, employeur, facture et états sociaux. PopulationManager : ajout de recherche mutable, propriété vecteur/index inchangée. ProfessionConfig : extension métier avec valeurs par défaut, compatible avec les agrégats existants. CSVProfessionProvider : méthode de fusion additionnelle, ancien load conservé. CitizenFactory : validation de couverture des âges et poids avant génération. RandomEngine : refus de poids négatifs/non finis/nuls. Makefile : compilation séparée, dépendances, cibles conservées et ajoutées. Aucun des six fichiers de tests d’origine n’a été supprimé ou réécrit.

## Extension V1.1 — mode joueur et présentation SFML

Les entrées V1 ci-dessus documentent la livraison précédente. Les nouveaux paramètres/actions et limites sont détaillés dans political-actions.md et decision-impact.md.

| Type / header | Implémentation | Responsabilité / API | Tests | État |
|---|---|---|---|---|
| `PlayerSession` — `include/dictatorsim/core/PlayerSession.hpp` | `src/core/PlayerSession.cpp` | Possède Simulation et contrôleur humain; advance/setSpeed/decide/respond/notify; accumulateur et file de crises | PlayerSessionTest / PlayerActionsTest / EventSeverityTest / PlayerDecisionImpactTest / PlayerCoupTest | IMPLEMENTED (limites documentées) |
| `EventSeverity` — `include/dictatorsim/events/EventSeverity.hpp` | `type simple; pas de .cpp` | Quatre niveaux de gravité | PlayerSessionTest / PlayerActionsTest / EventSeverityTest / PlayerDecisionImpactTest / PlayerCoupTest | IMPLEMENTED (limites documentées) |
| `EventCategory` — `include/dictatorsim/events/EventCategory.hpp` | `type simple; pas de .cpp` | Cinq catégories | PlayerSessionTest / PlayerActionsTest / EventSeverityTest / PlayerDecisionImpactTest / PlayerCoupTest | IMPLEMENTED (limites documentées) |
| `PlayerNotification` — `include/dictatorsim/events/PlayerNotification.hpp` | `src/events/PlayerNotification.cpp` | État de notification; EventClassifier::classify | PlayerSessionTest / PlayerActionsTest / EventSeverityTest / PlayerDecisionImpactTest / PlayerCoupTest | IMPLEMENTED (limites documentées) |
| `DecisionOption` — `include/dictatorsim/decisions/DecisionOption.hpp` | `type simple; pas de .cpp` | Libellé, description et PoliticalAction structurée | PlayerSessionTest / PlayerActionsTest / EventSeverityTest / PlayerDecisionImpactTest / PlayerCoupTest | IMPLEMENTED (limites documentées) |
| `DecisionHistory` — `include/dictatorsim/decisions/DecisionHistory.hpp` | `src/decisions/DecisionHistory.cpp` | Records avant/après, coût et observation J+30; add/observe/records | PlayerSessionTest / PlayerActionsTest / EventSeverityTest / PlayerDecisionImpactTest / PlayerCoupTest | IMPLEMENTED (limites documentées) |
| `PlayerDecisionController` — `include/dictatorsim/decisions/PlayerDecisionController.hpp` | `src/decisions/PlayerDecisionController.cpp` | Soumission validée et catalogue manuel; submit/catalog/history | PlayerSessionTest / PlayerActionsTest / EventSeverityTest / PlayerDecisionImpactTest / PlayerCoupTest | IMPLEMENTED (limites documentées) |
| `PoliticalConsequence` — `include/dictatorsim/politics/PoliticalConsequence.hpp` | `src/politics/PoliticalConsequence.cpp` | Exposition différée; PoliticalConsequenceSystem::update | PlayerSessionTest / PlayerActionsTest / EventSeverityTest / PlayerDecisionImpactTest / PlayerCoupTest | IMPLEMENTED (limites documentées) |
| `UITheme` — `include/dictatorsim/ui/UITheme.hpp` | `src/ui/UITheme.cpp` | Cache police/textes/formes; button/text/panel | essais graphiques SFML et captures | IMPLEMENTED (limites documentées) |
| `GameApplication` — `include/dictatorsim/ui/GameApplication.hpp` | `src/ui/GameApplication.cpp` | Fenêtre SFML, boucle et nouvelle partie; run | essais graphiques SFML et captures | IMPLEMENTED (limites documentées) |
| `DashboardScreen` — `include/dictatorsim/ui/DashboardScreen.hpp` | `src/ui/DashboardScreen.cpp` | Composition et briefings; draw/scroll | essais graphiques SFML et captures | IMPLEMENTED (limites documentées) |
| `DecisionPanel` — `include/dictatorsim/ui/DecisionPanel.hpp` | `src/ui/DecisionPanel.cpp` | Catalogue, confirmation, historique; draw/drawConfirmation | essais graphiques SFML et captures | IMPLEMENTED (limites documentées) |
| `EventPanel` — `include/dictatorsim/ui/EventPanel.hpp` | `src/ui/EventPanel.cpp` | Journal filtré visuellement par gravité; draw/scroll | essais graphiques SFML et captures | IMPLEMENTED (limites documentées) |
| `StatisticsPanel` — `include/dictatorsim/ui/StatisticsPanel.hpp` | `src/ui/StatisticsPanel.cpp` | Snapshots et courbe de stabilité; draw | essais graphiques SFML et captures | IMPLEMENTED (limites documentées) |
| `PauseMenu` — `include/dictatorsim/ui/PauseMenu.hpp` | `src/ui/PauseMenu.cpp` | Retour, nouvelle partie, fermeture; draw | essais graphiques SFML et captures | IMPLEMENTED (limites documentées) |
