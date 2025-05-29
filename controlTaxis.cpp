#include <iostream>
#include <string>
#include <iomanip>  //header para convertir float a 2 cifras
#include <unistd.h> //header para mover cursor en Linux
#include <limits>
#include <fstream>
#include <filesystem>
#include <vector>

// #include <windows.h> //header para mover cursor en Windows
using namespace std;

// la funcion GOTOXY cambia de a Linux y Windows(en este caso se esta utilizando la funcion para Linux)
void gotoxy(int x, int y)
{
    cout << "\033[" << y << ";" << x << "H";
}

struct Vehiculo
{
    string placa;
    string marca;
    string modelo;
    int year;
};

void guardarVehiculo(const vector<Vehiculo> &taxis, const string &nombreArchivo)
{
    ofstream archivo(nombreArchivo);
    for (const auto &taxi : taxis)
    {
        archivo << taxi.placa << " "
                << taxi.marca << " "
                << taxi.modelo << " "
                << taxi.year << endl;
    }
    archivo.close();
}

void cargarVector(vector<Vehiculo> &listaTaxis, const string &nombreArchivo)
{
    ifstream archivo(nombreArchivo);
    Vehiculo taxi;
    while (archivo >> taxi.placa)
    {
        archivo >> taxi.marca;
        archivo >> taxi.modelo;
        archivo >> taxi.year;
        listaTaxis.push_back(taxi);
    }
    archivo.close();
}

bool validarTaxiNuevo(const vector<Vehiculo> &listaTaxis, const string &placa, int year)
{

    // Validar si placa es Repetida
    for (const Vehiculo &taxi : listaTaxis)
    {
        if (taxi.placa == placa)
        {
            cout << "taxi con placas " << placa << " ya existe" <<endl;
            return false;
        }
    }

    // validar año de vehiculo
    if (year < 2011)
    {
        cout << "El año del vehiculo permitido es 2011 o superior" <<endl;
        return false;
    }

    return true; // si ambas condiciones se cumplen
}

int main()
{
    // Variables a Utilizar: Opcion
    char menu;
    char continuar;
    char resp;
    vector<Vehiculo> listaTaxis;
    Vehiculo taxi;
    string vehicleLabel[4] = {"1.Placa:", "2.Marca:", "3.Modelo:", "4.Año"};
    string temp; // variable temporal para convertir año de vehiculo a tipo numerico

    cargarVector(listaTaxis, "taxis.txt");

    system("clear");
    do
    {
        cout << "BIENVENIDO AL SISTEMA DE GESTION DE TAXIS 'TRUE-DRIVER'!" << endl;
        cout << "----------------------------------------------------" << endl;
        cout << "Ingrese la opción de la operacion que desea realizar:" << endl;
        cout << "Seleccione Una opcion Del menu que se presenta a continuacion:" << endl
             << endl;
        cout << "a. -- Registrar nuevo Taxi" << endl;
        cout << "b. -- Registrar nuevo Conductor" << endl;
        cout << "c. -- Enviar Taxi a cliente" << endl;
        cout << "d. -- Consulta de vehiculos en Ruta" << endl;
        cout << "e. -- Consulta de vehiculos disponibles" << endl;
        cout << "----------------------------------------------------\n"
             << endl;
        cout << "f. -- VER REPORTES" << endl;
        cout << "g. -- Salir\n"
             << endl;
        cout << "Ingrese la opcion seleccionada: ";
        cin >> menu;

        switch (menu)
        {
        case 'a':
            system("clear");
            cout << "*****************************************************************************************\n";
            cout << "Ingreso de nuevo Vehiculo.\n";

            // imprime campos/labels de Formulario//
            for (int i = 0; i < 4; i++)
            {
                gotoxy(3, 5 + i * 2);
                cout << vehicleLabel[i];
                gotoxy(18, 5 + i * 2);
                cout << "_______________________________";
            };
            cout << "\n \n";

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar el buffer

            gotoxy(19, 5);
            getline(cin, taxi.placa);
            gotoxy(19, 7);
            getline(cin, taxi.marca);
            gotoxy(19, 9);
            getline(cin, taxi.modelo);
            gotoxy(19, 11);
            getline(cin, temp);     // captura variable temporal para año
            taxi.year = stoi(temp); // asigna variable real a la estructura
            cout << "\n \n";
            // validar año del taxi
            // validar placa del taxi
            if ( validarTaxiNuevo(listaTaxis, taxi.placa, taxi.year) )
            {
                listaTaxis.push_back(taxi); // subimos el taxi guardado al vector
                guardarVehiculo(listaTaxis, "taxis.txt"); // agregamos el vector actualizado al archivo txt
                cout << "Nuevo Taxi Registrado." << endl;
            }else{
                cout<<"El Registro no se ha podido guardar."<< endl;
            }

            break;

        case 'b':
            system("clear");
            break;

        case 'c':
            system("clear");
            break;

        case 'd':
            system("clear");
            break;

        case 'e':
            system("clear");
            break;

        case 'f':
            system("clear");
            cout << "*********************************Reportes.***********************************" << endl;
            cout << "-----------------------------------------------------------------------------" << endl
                 << endl;
            cout << "Seleccione Una opcion Del menu que se presenta a continuacion:" << endl
                 << endl;
            cout << "a. -- Listado de viajes realizados por cada vehículo" << endl;
            cout << "b. -- Listado de viajes realizados por un vehículo específico" << endl;
            cout << "c. -- Ingresos totales obtenidos en un mes" << endl;
            cout << "d. -- Ingresos totales obtenidos en un mes por un vehículo específico" << endl;
            cout << "g. -- Salir\n"
                 << endl;
            cout << "Ingrese la opcion seleccionada: ";
            break;

        case 'g':
            system("clear");
            cout << " " << endl;
            cout << " " << endl;
            cout << " " << endl;
            cout << " " << endl;
            cout << " " << endl;
            cout << " " << endl;
            cout << " " << endl;
            cout << "Gracias por utilizar el sistema..." << endl;
            exit(0);
            break;
        }
        cout << "Desea realizar otra operacion? Presione Cualquier Tecla, Salir presione n o N." << endl;
        cin >> continuar;
        system("clear");

    } while (continuar != 'n' && continuar != 'N');
    cout << "Gracias por utilizar el sistema..." << endl
         << endl;

    return 0;
}