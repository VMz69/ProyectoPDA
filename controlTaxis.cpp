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

struct Conductor
{
    string dui;
    string nombre;
    string telefono;
    string direccion;
};

struct Vehiculo
{
    string placa;
    string marca;
    string modelo;
    int year;
    string categoria; // "Ejecutiva" o "Tradicional"
    string estado;    // "Disponible" o "En ruta"
    Conductor conductor;
};

// la funcion GOTOXY cambia de a Linux y Windows(en este caso se esta utilizando la funcion para Linux)
void gotoxy(int x, int y)
{
    cout << "\033[" << y << ";" << x << "H";
}

void guardarVehiculo(const vector<Vehiculo> &taxis, const string &nombreArchivo)
{
    ofstream archivo(nombreArchivo);
    for (const auto &taxi : taxis)
    {
        archivo << "# Vehiculo" << endl
                << taxi.placa << endl
                << taxi.marca << endl
                << taxi.modelo << endl
                << taxi.year << endl
                << taxi.categoria << endl
                << taxi.estado << endl
                << "# Conductor" <<endl
                << taxi.conductor.dui<<endl
                << taxi.conductor.nombre<<endl
                << taxi.conductor.telefono<<endl
                << taxi.conductor.direccion<<endl
                << endl;
    }
    archivo.close();
}

void cargarVector(vector<Vehiculo> &listaTaxis, const string &nombreArchivo)
{
    ifstream archivo(nombreArchivo);
    Vehiculo taxi;
   
    string linea; //para almacenar temporalmente cada línea leída

    while (getline(archivo, linea)) {
        if (linea == "# Vehiculo") {
            getline(archivo, taxi.placa);
            getline(archivo, taxi.marca);
            getline(archivo, taxi.modelo);
            string yearStr; //variable temporal para convertir año a int
            getline(archivo, yearStr);
            taxi.year = stoi(yearStr);
            
            getline(archivo, taxi.categoria);
            getline(archivo, taxi.estado);
            
            listaTaxis.push_back(taxi);
        }
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
            cout << "taxi con placas " << placa << " ya existe" << endl;
            return false;
        }
    }

    // validar año de vehiculo
    if (year < 2010)
    {
        cout << "El año del vehiculo permitido es 2010 o superior" << endl;
        return false;
    }

    return true; // si ambas condiciones se cumplen
}

int main()
{
    // Variables a Utilizar: Opcion
    char menu;
    char continuar;
    // char resp;
    vector<Vehiculo> listaTaxis;
    Vehiculo taxi;
    string vehicleLabel[4] = {"1.Placa:", "2.Marca:", "3.Modelo:", "4.Año"};
    string conductorLabel[4] = {"1.Documento de Identidad", "2.Nombre Completo:", "3.Telefono/Cel:", "4.Dirección"};
    string temp; // variable temporal para convertir año de vehiculo a tipo numerico

    cargarVector(listaTaxis, "CARS_STORAGE.txt");

    system("clear");
    do
    {
        cout << "BIENVENIDO AL SISTEMA DE GESTION DE TAXIS 'TRUE-DRIVER'!" << endl;
        cout << "----------------------------------------------------" << endl;
        cout << "Ingrese la opción de la operacion que desea realizar:" << endl;
        cout << "Seleccione Una opcion Del menu que se presenta a continuacion:" << endl
             << endl;
        cout << "a. -- Registrar nuevo Taxi y Conductor" << endl;
        cout << "b. -- Enviar Taxi a cliente" << endl;
        cout << "c. -- Consulta de vehiculos en Ruta" << endl;
        cout << "d. -- Consulta de vehiculos disponibles" << endl;
        cout << "----------------------------------------------------\n"
             << endl;
        cout << "e. -- VER REPORTES" << endl;
        cout << "z. -- Salir\n"
             << endl;
        cout << "Ingrese la opcion seleccionada: ";
        cin >> menu;

        switch (menu)
        {
        case 'a':
            //INGRESO DE TAXI NUEVO
            system("clear");
            cout << "**********************************************************************\n";
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

            // asignar categoria
            if (taxi.year >= 2015)
            {
                taxi.categoria = "Ejecutiva";
            }
            else
            {
                taxi.categoria = "Tradicional";
            }
            // asignar estado como disponible
            taxi.estado = "Disponible";

            if (validarTaxiNuevo(listaTaxis, taxi.placa, taxi.year)) // Validar año y placa del taxi
            {
               cout<<"El registro de Taxi cumple con los requerimientos" <<endl;
            }
            else
            {
                cout << "El Registro no se ha podido guardar." << endl;
                break;
            }

            //INGRESO DE CONDUCTOR ASOCIADO
            system("clear");
            cout << "**********************************************************************\n";
            cout << "\n<<<Registro de Conductor Asociado a taxi con Placas>>> " <<"<<" << taxi.placa << ">>\n";
            cout << "---------------------------------------------------------------------- \n";
            cout << "\n";
            // imprime campos/labels de Formulario//
            for (int i = 0; i < 4; i++)
            {
                gotoxy(3, 8 + i * 2);
                cout << conductorLabel[i];
                gotoxy(30, 8 + i * 2);
                cout << "_________________________________________________";
            };

            gotoxy(31, 8);
            getline(cin, taxi.conductor.dui);
            gotoxy(31, 10);
            getline(cin, taxi.conductor.nombre);
            gotoxy(31, 12);
            getline(cin, taxi.conductor.telefono);
            gotoxy(31, 14);
            getline(cin, taxi.conductor.direccion);     
            cout << "\n \n";

            listaTaxis.push_back(taxi);                      // subimos el taxi guardado al vector
            guardarVehiculo(listaTaxis, "CARS_STORAGE.txt"); // agregamos el vector actualizado al archivo txt

            system("clear");
            cout << "**********************************************************************\n";
            cout<<"Nuevo Taxi registrado nexitosamente."<<endl;
            cout<<"Placas:        "<< taxi.placa << endl;
            cout<<"Conductor:     "<< taxi.conductor.nombre <<endl;

            break;

        case 'b':
            system("clear");
            cout << "*****************************************************************************************\n";
            cout << "Ingreso de nuevo Conductor.\n";

            // imprime campos/labels de Formulario//
            for (int i = 0; i < 4; i++)
            {
                gotoxy(3, 5 + i * 2);
                cout << conductorLabel[i];
                gotoxy(30, 5 + i * 2);
                cout << "____________________________________________________";
            };
            cout << "\n \n";

            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // limpiar el buffer

            gotoxy(31, 5);
            cin >> temp;

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

        case 'z':
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
        cout << "\n Desea realizar otra operacion? Presione Cualquier Tecla, Salir presione n o N." << endl;
        cin >> continuar;
        system("clear");

    } while (continuar != 'n' && continuar != 'N');
    cout << "Gracias por utilizar el sistema..." << endl
         << endl;

    return 0;
}