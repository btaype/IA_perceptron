#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
using namespace std;
int recor = 1000;
struct ptron {
public:

    vector<double>pesos;
    bool estado = 0;
    int name = 0;
    double te = 0;
    ptron(int n, double ts1, vector<double> peso) {
        this->name = n;
        this->te = ts1;
        this->pesos = peso;


    }

    bool probar(vector<int>p) {

        double sum = 0;
        for (int i = 1; i < p.size(); i++) {
            sum = sum + (p[i] * pesos[i - 1]);
        }
        if (sum > 0) {
            return 1;
        }
        return 0;

    }

    void cambia_peso(vector<vector<int>>p) {
        bool rest = 1;
        while (rest) {
            for (int i = 0; i < recor; i++) {
                bool resultado = probar(p[i]);
                if ((name == p[i][0]) != resultado) {

                    for (int h = 0; h < pesos.size(); h++) {
                        int e1 = !resultado;
                        int e2 = resultado;
                        pesos[h] = pesos[h] + (te * (e1 - e2) * p[i][h + 1]);


                    }
                }
            }
            if (!rest) {
                rest = 1;
            }
            else {
                rest = 0;
            }

        }

    }
    void buscar(vector<int>p) {

        if (probar(p)) {
            estado = 1;
            return;

        }
        estado = 0;
        return;

    }
    void limiar() {
        estado = 0;
    }
};

void escribirCSV(vector<vector<int>> datos, const string nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        for (const auto& fila : datos) {
            for (size_t i = 0; i < fila.size(); ++i) {
                archivo << fila[i];
                if (i != fila.size() - 1)
                    archivo << ",";
            }
            archivo << endl;
        }
        archivo.close();
        cout << "Datos guardados en " << nombreArchivo << endl;
    }
    else {
        cout << "No se pudo abrir el archivo " << nombreArchivo
            << " para escritura." << endl;
    }
}

void escribirCSV2(vector<vector<double>> datos, const string nombreArchivo) {
    ofstream archivo(nombreArchivo);
    if (archivo.is_open()) {
        for (const auto& fila : datos) {
            for (size_t i = 0; i < fila.size(); ++i) {
                archivo << fila[i];
                if (i != fila.size() - 1)
                    archivo << ",";
            }
            archivo << endl;
        }
        archivo.close();
        cout << "Datos guardados en " << nombreArchivo << endl;
    }
    else {
        cout << "No se pudo abrir el archivo " << nombreArchivo
            << " para escritura." << endl;
    }
}
void hilos1(ptron& d, vector<vector<int>>p) {

    d.cambia_peso(p);
}
void hilos2(ptron& d, int t, vector<vector<int>>p) {

    d.buscar(p[t]);
}
int main()
{
    vector<vector<double>> pe;
    vector<vector<int>> pe1;

    ifstream file("C:\\mnist.csv");
    vector<vector<int>> data;
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo" << std::endl;
        return 1;
    }
    std::string line;
    while (getline(file, line)) {
        std::vector<int> row;
        std::stringstream lineStream(line);
        std::string cell;
        bool ac = 0;
        while (std::getline(lineStream, cell, ',')) {
            int tr = std::stoi(cell);
            if (ac) {
                if (tr > 127) {
                    tr = 1;
                }
                else {
                    tr = 0;
                }

            }
            ac = 1;
            row.push_back(tr);
        }
        row.insert(row.begin() + 1, 1);
        pe1.push_back(row);
    }

    ifstream file2("C:\\pesost.csv");

    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo" << std::endl;
        return 1;
    }
    std::string line2;
    while (getline(file2, line2)) {
        std::vector<double> row;
        std::stringstream lineStream(line2);
        std::string cell;

        while (std::getline(lineStream, cell, ',')) {
            row.push_back(std::stod(cell));
        }

        pe.push_back(row);
    }

    vector<ptron> percep;
    int neuronas = 10;
    for (int i = 0; i < neuronas; i++) {

        percep.push_back(ptron(i, 0.5, pe[i]));


    }
    vector<vector<int>> total;
    for (int yoo = 0; yoo < 50; yoo++) {
        vector<thread> threads;
        for (int i = 0; i < neuronas; i++) {
            threads.emplace_back(hilos1, ref(percep[i]), pe1);
        }
        for (auto& th : threads) {
            if (th.joinable()) {
                th.join();
            }
        }
        

        int entr = -1;

        int cont_si = 0;
        int cont_no = 0;
        threads.clear();

        for (int i = 0; i < 1000; i++) {
            vector<thread> threads1;
            ;
            for (int j = 0; j < neuronas; j++) {
                threads1.emplace_back(hilos2, ref(percep[j]), i, pe1);
            }
            for (auto& th : threads1) {
                if (th.joinable()) {
                    th.join();
                }
            }
            threads1.clear();
            int contrr = 0;
            for (int j = 0; j < neuronas; j++) {
                contrr = contrr + percep[j].estado;

            }
            if (pe1[i][0] < neuronas) {

                if (contrr == 1 && percep[pe1[i][0]].estado == 1) {
                    cont_si++;

                }
                else {
                    cont_no++;
                }
            }
            for (int J = 0; J < neuronas; J++) {


                percep[J].limiar();
            }
            cout << "sali:"<<i<<endl;
            
        }
        total.push_back({ cont_si,cont_no });
        cout << "yata\n";
        cout << '\n' << "Si" << cont_si << '\n';
        cout << '\n' << "No" << cont_no << '\n';
        
        pe.clear();
        for (int j = 0; j < neuronas; j++) {
            pe.push_back(percep[j].pesos);

        }
        escribirCSV2(pe, "D:\\pesosss"+to_string(yoo)+".csv");
    }
    
    escribirCSV(total, "D:\\error.csv"); 
    /*
    ptron cr1(1,0.5, pe[0]);

    cr1.cambia_peso(pe1);
    for (int i = 0; i < cr1.pesos.size(); i++) {
        cout << cr1.pesos[i] << ' ';
    }
    pe.clear();
    pe.push_back(cr1.pesos);
    cr1.buscar(pe1[59265]);
    cout << "\nel estado:  " << cr1.estado;
    escribirCSV(pe, "E:\\pesost1.csv");*/
}
