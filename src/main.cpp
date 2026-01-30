// --------- SETUP ---------
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cctype>

using std::string;

// ----------------------------------- MAIN -----------------------------------

class Data
{
    public:
        int g; int m; int a;

        Data(int g = 1, int m = 1, int a = 2000)
        {
            this->g = g; 
            this->m = m; 
            this->a = a;
        }

        
        void stampa_data()
            {std::cout << this->g << '/' << this->m + 1 << '/' << this->a + 1900;}
};

Data data_inizio()
{
    time_t now = std::time(NULL);
    tm* data = std::localtime(&now);

    int g = data->tm_mday;
    int m = data->tm_mon;
    int a = data->tm_year;

    return {g, m, a};
}

Data data_fine()
{
    time_t now = std::time(NULL);
    now += 30 * 24 * 60 * 60; // Durata prestito: 30g
    
    tm* data = std::localtime(&now);

    int g = data->tm_mday;
    int m = data->tm_mon;
    int a = data->tm_year;

    return {g, m, a};
}

struct Prestito
{
    int codice_utente;
    int codice_libro;
    Data inizio;
    Data fine;
};


class Libro
{
    private:
        string titolo;
        string autore;
        int codice;
        bool disponibile;
        std::vector<Prestito> cronologia_prestiti;

    public:
        Libro(string titolo, string autore, int codice)
        {
            // Conversioni in minuscolo
            for (char& carattere : titolo) carattere = std::tolower(static_cast<unsigned char>(carattere));
            for (char& carattere : autore) carattere = std::tolower(static_cast<unsigned char>(carattere));
            
            this->titolo = titolo;  
            this->autore = autore;  
            this->codice = codice; 
            this->disponibile = true;
        }

        // ------------- Getter -------------
        
        int get_codice()
            const {return this->codice;}

        string get_titolo()
            const {return this->titolo;}

        bool get_disponibilita()
            const {return this->disponibile;}
        
        int get_utente_in_prestito() 
        {
            if (!this->disponibile)
                // Restituisce il codice dell'utente che ha attualmente in prestito il libro
                return this->cronologia_prestiti[this->cronologia_prestiti.size() - 1].codice_utente;
            else
                std::cout << "\nIl libro non è in prestito!\n";
                return -1;
        }
        
        string get_autore()
            const {return this->autore;}

        int get_n_prestiti()
            const {return this->cronologia_prestiti.size();}

        // ---------------- Metodi -----------
        
        bool presta()
        {
            if (this->disponibile) {
                this->disponibile = false;
                return true;
            }
            else return false;
        }

        void restituisci()
            {this->disponibile = true;}

        void stampa_info()
        {
            std::cout << "Titolo: " << this->titolo
                      << ", Autore: " << this->autore
                      << ", Codice: " << this->codice
                      << ", n. prestiti: " << this->cronologia_prestiti.size()
                      << ", " << ((this->disponibile) ? "DISPONIBILE" : "NON DISPONIBILE") << ";\n";
        }

        void aggiungi_elemento_cronologia(Prestito elemento)
            {this->cronologia_prestiti.push_back(elemento);}

        void stampa_cronologia()
        {
            std::cout << "\n\nCronologia prestiti [" << this->titolo << "]:\n";
            for (Prestito prestito : this->cronologia_prestiti)
            {
                std::cout << "\nUtente: " << prestito.codice_utente;
                std::cout << ", Data inizio prestito: "; prestito.inizio.stampa_data();
                std::cout << ", Data fine  prestito: "; prestito.fine.stampa_data(); 
                std::cout<< ";\n";
            }
        }
};

class Utente
{
    private:
        int codice {};
        string nome;
        std::vector<Prestito> libri_in_prestito;
        std::vector<Prestito> cronologia_prestiti;

    public:
        Utente(int codice, string nome)
        {
            for (char& carattere : nome) carattere = std::tolower(static_cast<unsigned char>(carattere));

            this->codice = codice;  this->nome = nome;
        }

        // -------------- Getter -------------

        string get_nome()
            const {return this->nome;}

        int get_codice()
            const {return this->codice;}

        int get_n_libri_in_prestito()
            const {return this->libri_in_prestito.size();}

        // ------------- Metodi --------------

        void stampa_info()
        {
            std::cout << "Nome: " << this->nome
                      << ", Codice: " << this->codice
                      << ", N. prestiti totali: " << this->cronologia_prestiti.size()
                      << ", N. libri in prestito: " << this->libri_in_prestito.size() << ";\n";
        }

        bool puo_prendere_libro(string codice_libro)
        {
            bool puo {true};
            if (this->libri_in_prestito.size() > 3) puo = false;

            return puo;
        }

        void aggiungi_prestito(Prestito prestito)
        {
            this->libri_in_prestito.push_back(prestito);
            this->cronologia_prestiti.push_back(prestito);
        }

        bool rimuovi_prestito(int codice_libro)
        {
            bool possibile {false};
            for (int i = 0; i < this->libri_in_prestito.size(); i++)
            {
                if (this->libri_in_prestito[i].codice_libro == codice_libro) 
                {
                    // Elimina il libro dai libri attualmente in prestito
                    this->libri_in_prestito.erase(this->libri_in_prestito.begin() + i);

                    possibile = true;
                }
            }

            return possibile;
        }

        void stampa_prestiti()
        {
            std::cout << "Prestiti libri [" << this->nome << " - " << this->codice << "]:\n";
            for (Prestito prestito : this->cronologia_prestiti)
            {
                std::cout << "\nLibro: " << prestito.codice_libro; 
                std::cout << ", Data inizio prestito: "; prestito.inizio.stampa_data();
                std::cout << ", Data fine prestito: "; prestito.fine.stampa_data();
                std::cout << ";\n";
            }
        }
};

class Biblioteca
{
    private:
        std::vector<Libro> libri;
        std::vector<Utente> utenti;

    public:
        void aggiungi_libro()
        {
            string titolo {}, autore {};
            
            std::cout << "\n\nInserire il titolo: "; getline(std::cin, titolo);
            std::cout << "Inserire l'autore: ";      getline(std::cin, autore);


            if (titolo.length() > 3 && titolo.length() < 50 && autore.length() > 2 && autore.length() < 25) // Controlla la lunghezza di "titolo" e "autore"
            {
                // Assegna un codice al libro

                bool codice_utilizabile {true};
                int codice {};

                do
                {
                    codice = rand() % 10000000;
                    // Controlla che il codice non sia già stato utilizzato
                    for (Libro libro : this->libri)
                        if (libro.get_codice() == codice) codice_utilizabile = false;

                } while (!codice_utilizabile);
                    
                if (codice_utilizabile)
                {
                    this->libri.push_back(Libro(titolo, autore, codice)); // Crea il libro e lo aggiunge alla lista dei libri
                    std::cout << "Libro aggiunto con successo!\n";
                }
            }

            else
                std::cout << "Errore! I dati inseriti non sono validi!\n";
        }

        void aggiungi_utente()
        {
            string nome {};
            std::cout << "\n\nInserire il nome e cognome: "; getline(std::cin, nome);
        
            if (nome.length() > 7 && nome.length() < 30) // Controllo dati
            {
                // Assegnamento codice

                bool codice_utilizabile {true};
                int codice {};

                do
                {
                    codice = rand() % 10000000;
                    
                    for (Utente utente : this->utenti)
                        if (utente.get_codice() == codice) codice_utilizabile = false;
            
                } while (!codice_utilizabile);

                this->utenti.push_back(Utente(codice, nome)); // Crea l'utente e lo aggiunge ad utenti
                this->utenti[(this->utenti.size() - 1)].stampa_info();
            }

            else
                std::cout << "Errore! I dati inseriti non sono validi!\n";
        }

        void trova_libro_stampa()
        {
            string opzione_IN {};
            int opzione {};

            string titolo {""};
            string codice {""};
            
            std::cout << "\n\nPreferisci inserire il TITOLO(1) o il CODICE(2)?: "; getline(std::cin, opzione_IN);
            
            try {opzione = std::stoi(opzione_IN);}
            catch(const std::exception& e) {std::cout << "Errore! Inserire un opzione valida (1/2)." << '\n'; return;}

            // Controllo opzione
            if (opzione == 1 || opzione == 2)
            {
                if (opzione == 1) // Tramite TITOLO
                {
                    std::cout << "Inserire titolo: "; getline(std::cin, titolo);
                    // Conversione in minuscolo
                    for (char& carattere : titolo) carattere = std::tolower(static_cast<unsigned char>(carattere));
                }

                if (opzione == 2) // Tramite CODICE
                {
                    std::cout << "Inserire codice: "; getline(std::cin, codice);
                }

                // Ricerca libro

                bool trovato = {false};
                for (Libro& libro : this->libri)
                {
                    if (libro.get_titolo() == titolo || std::to_string(libro.get_codice()) == codice)
                    {
                        // Stampa le informazioni del libro
                        libro.stampa_info();

                        trovato = true;
                    }
                }

                // Se il libro non è stato trovato

                if (!trovato)
                    std::cout << "\nNessun libro trovato con questo " << ((opzione == 1) ? "titolo" : "codice") << "!\n";
            }

            else
                std::cout << "Errore! Opzione non valida.\n";
        }

        void trova_utente_stampa()
        {
            string opzione_IN {};
            int opzione {};

            string nome {};
            string codice {};

            std::cout << "\n\nPreferisci inserire il NOME&COGNOME(1) o il CODICE(2)?: "; getline(std::cin, opzione_IN);
            
            try {opzione = std::stoi(opzione_IN);}
            catch(const std::exception& e) {std::cout << "Errore! Inserire un opzione valida (1/2)." << '\n'; return;}


            // Controllo opzione
            if (opzione == 1 || opzione == 2)
            {
                if (opzione == 1) // Tramite TITOLO
                {
                    std::cout << "Inserire il nome: "; getline(std::cin, nome);
                    // Conversione in minuscolo
                    for (char& carattere : nome) carattere = std::tolower(static_cast<unsigned char>(carattere));
                }

                if (opzione == 2) // Tramite CODICE
                {
                    std::cout << "Inserire codice: "; getline(std::cin, codice);
                }

                // Ricerca libro

                bool trovato = {false};
                for (Utente& utente : this->utenti)
                {
                    if (utente.get_nome() == nome || std::to_string(utente.get_codice()) == codice)
                    {
                        utente.stampa_info();
                        trovato = true;
                    }
                }

                // Se il libro non è stato trovato
                
                if (!trovato)
                {
                    std::cout << "\nNessun utente trovato con questo " << ((opzione == 1) ? "nome" : "codice") << "!\n";
                }
            }

            else
                std::cout << "Errore! Opzione non valida.\n";
        }

        void presta_libro()
        {
            string codice_utente {};
            string codice_libro {};

            bool utente_trovato {false};
            bool libro_trovato {false};

            std::cout << "\n\nCodice utente: "; getline(std::cin, codice_utente);
            std::cout << "Codice libro: ";  getline(std::cin, codice_libro);

            for (Utente& utente : this->utenti)
            {
                if (std::to_string(utente.get_codice()) == codice_utente) 
                {
                    if (utente.puo_prendere_libro(codice_libro))
                    {
                        for (Libro& libro : this->libri)
                        {
                            if (std::to_string(libro.get_codice()) == codice_libro)
                            {
                                if (libro.presta())
                                {
                                    Prestito prestito = {std::stoi(codice_utente), std::stoi(codice_libro), data_inizio(), data_fine()};
                                    libro.aggiungi_elemento_cronologia(prestito);
                                    utente.aggiungi_prestito(prestito);

                                    {
                                        std::cout << "Prestito avvenuto con successo!\n"
                                                << "Libro: " << libro.get_titolo()
                                                << ", Utente: " << utente.get_nome()
                                                << ", Data inizio prestito: "; prestito.inizio.stampa_data();
                                        std::cout << ", Data FINE PRESTITO: "; prestito.fine.stampa_data();
                                        std::cout << ";\n";
                                    }
                                }
                                
                                else
                                    std::cout << "\nIl libro NON è disponibile!\n";

                                libro_trovato = true;
                            }
                        }
                    }
                    utente_trovato = true;
                }
            }

            // Gestione ERRORI

            if (!utente_trovato && !libro_trovato)
                std::cout << "\nErrore!\n";

            else if (utente_trovato && !libro_trovato)
                std::cout << "\nErrore! Libro non trovato.\n";
        }

        void restituisci_libro()
        {
            string codice_libro {};
            std::cout << "\n\nCodice libro: "; getline(std::cin, codice_libro);

            bool libro_trovato {false};
            
            for (Libro& libro : this->libri)
            {
                if (std::to_string(libro.get_codice()) == codice_libro)
                {
                    if (!libro.get_disponibilita())
                    {
                        for (Utente& utente : this->utenti)
                        {
                            if (utente.get_codice() == libro.get_utente_in_prestito())
                            {
                                libro.restituisci();
                                utente.rimuovi_prestito(libro.get_codice());

                                std::cout << "Libro restituito con successo!\n";

                                libro_trovato = true;
                            }
                        }
                    }
                }
                if (!libro_trovato)
                    std::cout << "\nErrore! Libro non trovato.\n";
            }
        }

        void stampa_cronologia_utente()
        {
            string codice_utente {};
            std::cout << "\n\nCodice utente: "; getline(std::cin, codice_utente);

            for (Utente& utente : this->utenti)
            {
                if (std::to_string(utente.get_codice()) == codice_utente)
                {
                    utente.stampa_prestiti();
                }
            }
        }

        void stampa_cronologia_libro()
        {
            string codice_libro {};
            std::cout << "\n\nCodice libro: "; std::cin >> codice_libro;

            for (Libro& libro: this->libri)
            {
                if (std::to_string(libro.get_codice()) == codice_libro)
                {
                    std::cout << "\nCronologia prestiti:\n";
                    libro.stampa_cronologia();
                }
            }
        }
};



// -------------- MAIN -------------

int main()
{
    std::srand(std::time(0));

    std::vector<string> opzioni = {
        "Presta libro",
        "Restituisci libro",
        "Aggiungi utente",
        "Aggiungi libro",
        "Cerca libro",
        "Cerca utente",
        "Stampa cronologia utente",
        "Stampa cronologia libro"
    };

    Biblioteca biblioteca;

    while (true)
    {
        // Stampa menù
        std::cout << "\n------------------- OPZIONI -------------------\n\n";
        for (int i = 0; i < opzioni.size(); i++)
            std::cout << i + 1 << " - " << opzioni[i] << ".\n";
        
        // Opzione utente
        string opzione_scelta_IN {};
        std::cout << "\nInserisci opzione: "; getline(std::cin, opzione_scelta_IN);

        int opzione_scelta {};

        try {opzione_scelta = std::stoi(opzione_scelta_IN);}
        catch(const std::invalid_argument& e)
        {
            std::cout << "\nErrore! Opzione non valida.\n";
        }
        
        // Azione

        switch (opzione_scelta)
        {
            case 1: biblioteca.presta_libro();             break;
            case 2: biblioteca.restituisci_libro();        break;
            case 3: biblioteca.aggiungi_utente();          break;
            case 4: biblioteca.aggiungi_libro();           break;
            case 5: biblioteca.trova_libro_stampa();       break;
            case 6: biblioteca.trova_utente_stampa();      break;
            case 7: biblioteca.stampa_cronologia_utente(); break;
            case 8: biblioteca.stampa_cronologia_libro();  break;
            
            default:
                std::cout << "\nErrore! Opzione non valida.\n";
                break;
        }
    }

    return 0;
}
