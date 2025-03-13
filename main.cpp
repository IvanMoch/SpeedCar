#include <iostream>
#include <string.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Declaración de las variables y estructuras que se implementaran en el proyecto------------------------
struct Vehiculo
{
    char modelo[45];
    char conductor[45];
    int sectorActual;
    char placa[20];
    int edad;
    int ci;
    bool ocupado;
};

struct Usuario
{
    int ci;
    char nombre[45];
    int sectorActual;
};

struct Sector
{
    char nombre[100];
};

int MenuPrincipal();
void subMenuGestion();
int subMenuEntidad(string entidad);
void subMenuServicio();
void crearEntidad(string entidad);
void mostrarEntidad(string entidad);
void modificarEntidad(string entidad);
void BorrarEntidad(string entidad);
bool verificarExistencia(string entidad, int ci);
bool verificarExistenciaSector(int id);
void mostrarVehiculosCerca(int ci);
void ubicacionVehiculo();
void solicitudTraslado(int ciUsuario);
void desocuparVehiculo(int ci);
void mostrarSectores();
string mostrarSector(int idObjetivo);
void leerDistancias();
void inicializarGrafo();
void llenarGrafo();
void mostrarMatriz();
void encontrarCamino(int sectorOrigen, int sectorDestino);
void cargarGrafo();
int obtenerId();
Vehiculo infVehiculo();
Usuario infUsuario();
int ci;

fstream DB;

Usuario usuario;
Vehiculo vehiculo;
Sector sector;
vector<int> ocupados;
int pesoTotal = 0;
vector<pair<int, int>> ruta;
vector<int> nodos;
unordered_map<int, int> nodoIndice;
vector<vector<int>> grafo;
const int INF = -1;

//-------------------------------------------------------------------------------------

// Función main------------------------------------------------------------------------

// int main(){

//     int aux;

//     cargarGrafo();

//     do
//     {
//         string entidad;
//         aux = MenuPrincipal();

//         switch (aux)
//         {
//         case 1:
//             subMenuGestion();
//             break;
//         case 2:
//             subMenuServicio();
//             break;
//         default:
//             break;
//         }
//     } while (aux != 0);

//     cout << "Has salido de la aplicacion" << endl;

//     if(DB.is_open()){
//         DB.close();
//     }

//     return 0;
// }

int main()
{

    cargarGrafo();

    mostrarMatriz();

    encontrarCamino(1, 5);

    return 0;
}

//-----------------------------------------------------------------------------------

// Menu principal---------------------------------------------------------------------

/*Esta función muestra por pantalla las opciones del menú principal y retorna un numero que sirve como indicador
para indicar a que menú desea ir el usuario */

int MenuPrincipal()
{
    system("clear");
    int aux;

    cout << "BIENVENIDO A SpeedCar" << endl;
    cout << "1) Menu de gestion" << endl;
    cout << "2) menu de servicios" << endl;
    cout << "0) salir de la aplicacion" << endl;
    cout << "Ingrese su opcion ->";
    cin >> aux;

    return aux;
}

//-----------------------------------------------------------------------

// Sub menu del menu de gestión ------------------------------------------

/*Esta funcion muestra por pantalla las opciones que tiene el usuario para gestionar*/

void subMenuGestion()
{
    system("clear");
    int aux;
    string entidad;

    cout << "Desea Gestionar:" << endl;
    cout << "1)Usuario" << endl;
    cout << "2)Vehiculo" << endl;
    cout << "3)sector" << endl;
    cout << "Ingrese su opcion -> ";
    cin >> aux;

    switch (aux)
    {
    case 1:
        cout << "..." << endl;
        entidad = "Usuario";
        break;
    case 2:
        entidad = "Vehiculo";
        break;
    case 3:
        entidad = "Sector";
        break;
    default:
        cout << "Debe colocar una de las opciones especificadas" << endl;
        subMenuGestion();
        break;
    }

    aux = subMenuEntidad(entidad);

    switch (aux)
    {
    case 1:
        crearEntidad(entidad);
        break;
    case 2:
        mostrarEntidad(entidad);
        break;
    case 3:
        modificarEntidad(entidad);
        break;
    case 4:
        BorrarEntidad(entidad);
        break;
    case 5:
        cout << "salir" << endl;
        break;
    default:
        cout << "Debe colocar una de las opciones especificadas" << endl;
        subMenuGestion();
        break;
    }

    cout << "presiona enter para volver al menu principal" << endl;
    system("read -p '...' var");
}

// Menu para hacer el CRUD de la identidad seleccionada por el usuario-----------------------

/*Esta función muestra al usuario las operaciones CRUD que desee hacer y retorna un entero
para indicar cual fue la función que selecciono el usuario*/

int subMenuEntidad(string entidad)
{
    system("clear");
    int aux;

    if (!strcasecmp(entidad.c_str(), "Vehiculo") || !strcasecmp(entidad.c_str(), "Usuario"))
    {
        cout << "1)Crear " << entidad << endl;
        cout << "2)Mostrar " << entidad << endl;
        cout << "3)Modificar " << entidad << endl;
        cout << "4)Borrar " << entidad << endl;
        cout << "5)Salir del menu" << endl;
        cout << "Ingrese su opción -> ";
        cin >> aux;
        cout << endl;
    }

    if (!strcasecmp(entidad.c_str(), "Sector"))
    {
        cout << "1)Crear " << entidad << endl;
        cout << "4)Borrar " << entidad << endl;
        cout << "Ingrese su opción ->";
        cin >> aux;
    }

    return aux;
}

//----------------------------------------------------------------------------------

// Menu de servicios-----------------------------------------------------------------

/* Esta función muestra por pantalla las funciones que puede hacer el usuario en el menu servicio*/

void subMenuServicio()
{
    system("clear");
    int aux;
    cout << "Bienvenido al menu de servicio" << endl;
    cout << "1) Actualizar ubicacion" << endl;
    cout << "2) Solicitar traslado" << endl;
    cout << "3)Seleccionar vehiculo" << endl;
    cout << "4)Finalizar traslado" << endl;
    cout << "5)Volver al menu principal" << endl;
    cout << "ingrese su opcion -> ";
    cin >> aux;

    switch (aux)
    {
    case 1:
        ubicacionVehiculo();
        break;
    case 2:
        system("clear");
        cin.ignore();
        cout << "Coloque la cédula del usuario: ";
        cin >> ci;
        solicitudTraslado(ci);
        break;
    case 3:
        system("clear");
        cin.ignore();
        cout << "Coloque la cédula del usuario: ";
        cin >> ci;
        mostrarVehiculosCerca(ci);
        break;
    }
}

//----------------------------------------------------------------------------------

// CRUD------------------------------------------------------------------------------

void crearEntidad(string entidad)
{
    system("clear");

    if (!(strcasecmp(entidad.c_str(), "Vehiculo")))
    {
        Vehiculo vehiculo = infVehiculo(); // Almacena los datos en la variable global
        if (verificarExistencia(entidad, vehiculo.ci))
        {
            cout << "El usuario ingresado ya existe" << endl;
            system("read -p 'Presione enter para volver al menu principal...' var");
            DB.close();
            MenuPrincipal();
        }
        else
        {
            DB.open("vehiculos.dat", ios::out | ios::app | ios::binary);
            if (DB.is_open())
            {
                DB.write((char *)&vehiculo, sizeof(Vehiculo));
                DB.close();
                system("clear");
                cout << "-------------------" << endl;
                cout << "|Vehiculo guardado|" << endl;
                cout << "-------------------" << endl;
                system("read -p 'presione enter para continuar' var");
            }
            else
            {
                cerr << "No se pudo abrir el archivo vehiculos.dat" << endl;
            }
        }
    }

    if (!strcasecmp(entidad.c_str(), "Usuario"))
    {
        Usuario usuario = infUsuario(); // Almacena los datos en la variable global "usuario"
        if (verificarExistencia(entidad, usuario.ci))
        {
            cout << "El usuario ingresado ya existe" << endl;
            system("read -p 'Presione enter para volver al menu principal...' var");
            DB.close();
            MenuPrincipal();
        }
        else
        {
            DB.open("usuarios.dat", ios::out | ios::app | ios::binary);
            if (DB.is_open())
            {
                DB.write((char *)&usuario, sizeof(Usuario));
                DB.close();
                system("clear");
                cout << "------------------" << endl;
                cout << "|Usuario guardado|" << endl;
                cout << "------------------" << endl;
                system("read -p 'presione enter para continuar' var");
            }
            else
            {
                cerr << "No se pudo abrir el archivo usuarios.da" << endl;
            }
        }
    }

    if (!strcasecmp(entidad.c_str(), "Sector"))
    {
        int id;
        string nombreSector;
        cin.ignore();
        cout << "Ingrese el id del sector: ";
        cin >> id;
        cout << "Ingrese el nombre del sector: ";
        cin.ignore();
        getline(cin, nombreSector);

        if (verificarExistenciaSector(id))
        {
            cout << "El sector ya existe" << endl;
            system("read -p 'Presione enter para volver al menu principal...' var");
            DB.close();
            MenuPrincipal();
        }
        else
        {
            ofstream sectores("sectores.txt", ios::app);
            if (sectores.is_open())
            {
                sectores << id << "," << nombreSector << endl;
                sectores.close();
                system("clear");
                cout << "------------------" << endl;
                cout << "|Sector guardado|" << endl;
                cout << "------------------" << endl;
                system("read -p 'presione enter para continuar' var");
            }
            else
            {
                cerr << "No se pudo abrir el archivo sectores.txt" << endl;
            }
        }
    }
}

//-------------------------------------------------------------------------------

void mostrarEntidad(string entidad)
{
    system("clear");
    int ciEntidad;

    if (!strcasecmp(entidad.c_str(), "Vehiculo"))
    {
        /* Codigo para mostrar un vehiculo en vehiculos.da */

        cout << "CI del conductor que busca: ";
        cin >> ciEntidad;

        DB.open("vehiculos.dat", ios::in | ios::binary);

        cout << (DB.is_open()) << endl;

        if (DB.is_open())
        {
            while (DB.read((char *)&vehiculo, sizeof(Vehiculo)))
            {

                cout << vehiculo.ci << " " << vehiculo.conductor << " " << ciEntidad << endl;

                if (vehiculo.ci == ciEntidad)
                {
                    cout << "----------Información del conductor----------" << endl;
                    cout << "|nombre del conductor: " << vehiculo.conductor << "|" << endl;
                    cout << "|Modelo del vehiculo: " << vehiculo.modelo << "|" << endl;
                    cout << "|Placa del vehiculo: " << vehiculo.placa << "  |" << endl;
                    cout << "|Sector actual: " << mostrarSector(vehiculo.sectorActual) << "|" << endl;

                    if (vehiculo.ocupado)
                    {
                        cout << "|El conductor se encuentra haciendo una carrera|" << endl;
                    }
                    else
                    {
                        cout << "|El conductor esta libre para hacer una carrera|" << endl;
                    }

                    cout << "---------------------------------------------" << endl;

                    break;
                }
            }
            system("read -p 'presione enter para continuar' var");
        }
    }
    if (!strcasecmp(entidad.c_str(), "Usuario"))
    {

        cout << "CI del usuario que busca: ";
        cin >> ciEntidad;

        DB.open("usuarios.dat", ios::in | ios::binary);

        if (DB.is_open())
        {
            while (DB.read((char *)&usuario, sizeof(Usuario)))
            {

                cout << usuario.ci << " " << usuario.nombre << " " << ciEntidad << endl;

                if (usuario.ci == ciEntidad)
                {
                    cout << "----------Informacion del usuario----------" << endl;
                    cout << "|nombre del conductor: " << usuario.nombre << "|" << endl;
                    cout << "|Sector actual: " << mostrarSector(usuario.sectorActual) << "|" << endl;
                    cout << "---------------------------------------------" << endl;
                    break;
                }
            }
            system("read -p 'presione enter para continuar' var");
        }
    }

    DB.close();
}

//--------------------------------------------------------------------

void modificarEntidad(string entidad)
{
    system("clear");

    if (!strcasecmp(entidad.c_str(), "vehiculo"))
    {
        /* Codigo para ingresar un vehiculo en vehículos.da */
        bool encontrado = false;
        int ci;

        cin.ignore();
        cout << "Introduzca la cedula del conductor que desea modificar los datos: ";
        cin >> ci;

        DB.open("vehiculos.dat", ios::in | ios::out | ios::binary); // Relee y modifica, es decir, sobrescribe el archivo ya existente

        while (DB.read((char *)&vehiculo, sizeof(Vehiculo)))
        {
            if (vehiculo.ci == ci)
            {

                cout << "\tIngrese los nuevos datos" << endl;
                cout << "Nombre: ";
                cin.ignore();
                cin.getline(vehiculo.conductor, 45, '\n');
                cout << "Sector actual: ";
                mostrarSectores();
                cout << "Coloque el numero del sector -> ";
                cin >> vehiculo.sectorActual;
                cout << "Edad: ";
                cin >> vehiculo.edad;

                DB.seekp(-static_cast<int>(sizeof(Vehiculo)), ios::cur); // Metodo para regresar en el archivo para sobreescribir en el archivo (Indagar)
                DB.write((char *)&vehiculo, sizeof(Vehiculo));

                encontrado = true;
                break;
            }
        }

        if (!encontrado)
        {
            cout << "Vehiculo no encontrado" << endl;
        }
        DB.close();
    }

    if (!strcasecmp(entidad.c_str(), "usuario"))
    {
        bool encontrado = false;
        int ci;

        cin.ignore();
        cout << "Introduzca la cedula del usuario que desea modificar los datos: ";
        cin >> ci;

        DB.open("usuarios.dat", ios::in | ios::out | ios::binary); // Relee y modifica, es decir, sobrescribe el archivo ya existente

        while (DB.read((char *)&usuario, sizeof(Usuario)))
        {
            if (usuario.ci == ci)
            {

                cout << "\tIngrese los nuevos datos" << endl;
                cout << "Nombre: ";
                cin.ignore();
                cin.getline(usuario.nombre, 45, '\n');
                cout << "Sector actual: ";
                mostrarSectores();
                cout << "Coloque el numero del sector -> ";
                cin >> usuario.sectorActual;

                DB.seekp(-static_cast<int>(sizeof(Usuario)), ios::cur);
                DB.write((char *)&usuario, sizeof(Usuario));

                encontrado = true;
                break;
            }
        }

        if (!encontrado)
        {
            cout << "Usuario no encontrado" << endl;
        }
        DB.close();
    }
}

//------------------------------------------------------------------------

void BorrarEntidad(string entidad)
{
    system("clear");
    int ci;
    char nombre[100];

    if (!strcasecmp(entidad.c_str(), "vehiculo"))
    {
        bool encontrado = false;

        cin.ignore();
        cout << "Introduzca la cedula del conductor: ";
        cin >> ci;

        DB.open("vehiculos.dat", ios::in | ios::out | ios::binary);

        while (DB.read((char *)&vehiculo, sizeof(Vehiculo)))
        {
            if (vehiculo.ci == ci)
            {

                memset(vehiculo.conductor, '\0', sizeof(vehiculo.conductor));
                vehiculo.edad = 0;
                vehiculo.ci = 0;

                DB.seekp(-static_cast<int>(sizeof(Vehiculo)), ios::cur);
                DB.write(reinterpret_cast<char *>(&vehiculo), sizeof(Vehiculo));

                encontrado = true;
                cout << "Participante eliminado exitosamente" << endl;
                break;
            }
        }

        if (!encontrado)
        {
            cout << "Participante no encontrado" << endl;
        }
        DB.close();
    }

    if (!strcasecmp(entidad.c_str(), "usuario"))
    {

        bool encontrado = false;

        cin.ignore();
        cout << "Introduzca la cedula del usuario: ";
        cin >> ci;

        DB.open("usuarios.dat", ios::in | ios::out | ios::binary);

        while (DB.read((char *)&usuario, sizeof(Usuario)))
        {
            if (usuario.ci == ci)
            {

                memset(usuario.nombre, '\0', sizeof(usuario.nombre));
                usuario.ci = 0;

                DB.seekp(-static_cast<int>(sizeof(Usuario)), ios::cur);
                DB.write(reinterpret_cast<char *>(&usuario), sizeof(Usuario));

                encontrado = true;
                cout << "Participante eliminado exitosamente" << endl;
                break;
            }
        }

        if (!encontrado)
        {
            cout << "Participante no encontrado" << endl;
        }
        DB.close();
    }
}

//-----------------------------------------------------------------------

bool verificarExistencia(string entidad, int ci)
{

    bool existe = false;

    if (!strcasecmp(entidad.c_str(), "vehiculo"))
    {
        /* Codigo para ingresar un vehiculo en vehículos.da */
        DB.open("vehiculos.dat", ios::in | ios::binary);

        while (DB.read((char *)&vehiculo, sizeof(Vehiculo)))
        {
            if (vehiculo.ci == ci)
            {
                existe = true;
            }
        }
        DB.close();
    }
    if (!strcasecmp(entidad.c_str(), "usuario"))
    {
        DB.open("usuarios.dat", ios::in | ios::binary);

        while (DB.read((char *)&usuario, sizeof(Usuario)))
        {
            if (usuario.ci == ci)
            {
                existe = true;
            }
        }
        DB.close();
    }

    return existe;
}

bool verificarExistenciaSector(int id)
{

    bool existe = false;

    ifstream sectores("sectores.txt");

    string linea;
    int idSector;

    while (getline(sectores, linea))
    {

        stringstream ss(linea);
        char coma;

        ss >> idSector >> coma;

        if (idSector == id)
        {
            existe = true;
        }
    }
    sectores.close();
    return existe;
}

void ubicacionVehiculo()
{
    system("clear");
    char placa[15];
    int nuevoSector;
    bool encontrado = false;

    cout << "Ingrese la placa del vehiculo: ";
    cin.ignore();
    cin.getline(placa, 15, '\n');

    DB.open("vehiculos.dat", ios::in | ios::out | ios::binary);

    while (DB.read(reinterpret_cast<char *>(&vehiculo), sizeof(Vehiculo)))
    {
        if (strcmp(vehiculo.placa, placa) == 0)
        {
            cout << "\tPlaca encontrada" << endl;
            cout << "Ubicacion actual: " << mostrarSector(vehiculo.sectorActual) << endl;
            cout << "Ingrese el nuevo sector del vehiculo: ";
            mostrarSectores();
            cout << "Coloque el numero del sector -> ";
            cin >> nuevoSector;
            vehiculo.sectorActual = nuevoSector;

            DB.seekp(-static_cast<int>(sizeof(Vehiculo)), ios::cur);
            DB.write(reinterpret_cast<char *>(&vehiculo), sizeof(Vehiculo));

            encontrado = true;
            cout << "Ubicacion del vehiculo actualizada exitosamente." << endl;
            break;
        }
    }

    if (!encontrado)
    {
        cout << "Vehiculo no encontrado." << endl;
    }

    DB.close();
}

void solicitudTraslado(int ciUsuario)
{
    cout << "---------------------------" << endl;
    int destino;
    int origen;
    bool realizado = false;

    DB.open("usuarios.dat", ios::in | ios::out | ios::binary);
    Usuario usuario;
    Vehiculo vehiculo;

    while (DB.read((char *)&usuario, sizeof(Usuario)))
    {
        if (usuario.ci == ci)
        {
            origen = usuario.sectorActual;
            DB.close();

            DB.open("vehiculos.dat", ios::in | ios::out | ios::binary);

            while (DB.read((char *)&vehiculo, sizeof(Vehiculo)))
            {
                if (!vehiculo.ocupado && vehiculo.sectorActual == usuario.sectorActual)
                {
                    cout << "\tServicio Disponible en tu zona" << endl;
                    cout << "Sector de Origen: " << mostrarSector(vehiculo.sectorActual) << endl;
                    cout << "Ingrese el Destino: " << endl;
                    mostrarSectores();
                    cout << "Ingrese el numero del destino ->";
                    cin >> destino;

                    vehiculo.sectorActual = destino;
                    usuario.sectorActual = destino;

                    // Mover el puntero hacia atr\E1s para sobrescribir
                    DB.seekp(-static_cast<int>(sizeof(Vehiculo)), ios::cur);
                    DB.write(reinterpret_cast<char *>(&vehiculo), sizeof(Vehiculo));

                    realizado = true;
                    break;
                }
            }

            DB.close();
        }
    }

    if (realizado == 1)
    {
        cout << "----------REALIZANDO TRASLADO----------" << endl;
        encontrarCamino(origen, destino);
        system("read -p 'presione enter cuando termine el traslado' var");
        DB.open("usuarios.dat", ios::in | ios::out | ios::binary);
        if (DB.is_open())
        {
            while (DB.read((char *)&usuario, sizeof(Usuario)))
            {
                if (usuario.ci == ciUsuario)
                {
                    usuario.sectorActual = destino;
                    DB.seekp(-static_cast<int>(sizeof(Usuario)), ios::cur);
                    DB.write((char *)&usuario, sizeof(Usuario));
                    break;
                }
            }
        }
        DB.close();
    }
    else
    {
        cout << "El servicio no se encuentra disponible ahora mismo. Intentelo m\E1s tarde." << endl;
        system("read -p 'presione enter para continuar' var");
    }
}

void solicitarConductor(int ciUsuario, int ciConductor)
{
    cout << "---------------------------" << endl;
    int destino;
    bool realizado = false;

    DB.open("usuarios.dat", ios::in | ios::out | ios::binary);
    Usuario usuario;
    Vehiculo vehiculo;

    while (DB.read((char *)&usuario, sizeof(Usuario)))
    {
        if (usuario.ci == ciUsuario)
        {
            // Encontrado el usuario
            DB.close(); // Cerrar el archivo de usuarios antes de abrir el de veh\EDculos

            DB.open("vehiculos.dat", ios::in | ios::out | ios::binary);

            while (DB.read((char *)&vehiculo, sizeof(Vehiculo)))
            {
                if (!vehiculo.ocupado && vehiculo.sectorActual == usuario.sectorActual && vehiculo.ci == ciConductor)
                {
                    cout << "\tServicio Disponible en tu zona" << endl;
                    cout << "Sector de Origen: " << mostrarSector(vehiculo.sectorActual) << endl;
                    cout << "Ingrese el Destino: " << endl;
                    mostrarSectores();
                    cout << "Coloque el numero del sector -> ";
                    cin >> destino;

                    vehiculo.sectorActual = destino;
                    usuario.sectorActual = destino;
                    vehiculo.ocupado = true;

                    // Mover el puntero hacia atr\E1s para sobrescribir
                    DB.seekp(-static_cast<int>(sizeof(Vehiculo)), ios::cur);
                    DB.write(reinterpret_cast<char *>(&vehiculo), sizeof(Vehiculo));

                    realizado = true;
                    break;
                }
            }

            DB.close(); // Cerrar el archivo de veh\EDculos despu\E9s de la operaci\F3n
        }
    }

    if (realizado)
    {
        cout << "----------REALIZANDO TRASLADO----------" << endl;
        system("read -p 'presione enter cuando termine el traslado' var");
        DB.open("usuarios.dat", ios::in | ios::out | ios::binary);
        if (DB.is_open())
        {
            while (DB.read((char *)&usuario, sizeof(Usuario)))
            {
                if (usuario.ci == ci)
                {
                    usuario.sectorActual = destino;
                    DB.seekp(-static_cast<int>(sizeof(Usuario)), ios::cur);
                    DB.write((char *)&usuario, sizeof(Usuario));
                    break;
                }
            }
        }
        DB.close();
    }
    else
    {
        cout << "El servicio no se encuentra disponible ahora mismo. Intentelo m\E1s tarde." << endl;
        system("read -p 'presione enter para continuar' var");
    }
}

void mostrarVehiculosCerca(int ciUsuario)
{

    int sectorObjetivo;
    int ciConductor;
    bool usuarioEncontrado = false;
    bool vehiculoEncontrado = false;
    system("clear");

    DB.open("usuarios.dat", ios::in | ios::binary);

    if (DB.is_open())
    {
        while (DB.read((char *)&usuario, sizeof(Usuario)))
        {
            if (usuario.ci == ciUsuario)
            {
                sectorObjetivo = usuario.sectorActual;
                usuarioEncontrado = true;
            }
        }
    }

    DB.close();

    DB.open("vehiculos.dat", ios::in | ios::binary);

    if (DB.is_open())
    {
        while (DB.read((char *)&vehiculo, sizeof(Vehiculo)))
        {
            if (vehiculo.sectorActual == sectorObjetivo && !vehiculo.ocupado)
            {
                cout << "----------Información del conductor----------" << endl;
                cout << "|Cédula del conductor: " << vehiculo.ci << "|" << endl;
                cout << "|Nombre del conductor: " << vehiculo.conductor << "|" << endl;
                cout << "|Modelo del vehiculo: " << vehiculo.modelo << "|" << endl;
                cout << "|Placa del vehiculo: " << vehiculo.placa << "  |" << endl;
                cout << "|Sector actual: " << mostrarSector(vehiculo.sectorActual) << "|" << endl;
                cout << "---------------------------------------------" << endl;
                vehiculoEncontrado = true;
            }
        }
    }

    DB.close();

    if (vehiculoEncontrado)
    {
        cin.ignore();
        cout << "Indique la cédula del conductor que desees : ";
        cin >> ciConductor;
        solicitarConductor(ciUsuario, ciConductor);

        cout << "Realizando traslado, cuando llegues a tu destino presiona Enter" << endl;
        system("read -p '...' var");
        desocuparVehiculo(ciConductor);
    }
}

// Obtener los datos del vehiculo---------------------------------------------------

Vehiculo infVehiculo()
{
    // Resetear la estructura vehiculo
    Vehiculo vehiculo;

    cin.ignore();
    cout << "nombre del chofer: ";
    cin.getline(vehiculo.conductor, 45, '\n');
    cout << "modelo del vehiculo: ";
    cin.getline(vehiculo.modelo, 45, '\n');
    cout << "Placa del vehiculo: ";
    cin.getline(vehiculo.placa, 20, '\n');
    cout << "Sector actual en el que estas: " << endl;
    mostrarSectores();
    cout << "ingrese el numero de su sector actual -> ";
    cin >> vehiculo.sectorActual;
    cout << "edad del conductor: ";
    cin >> vehiculo.edad;
    cout << "CI del conductor: ";
    cin >> vehiculo.ci;

    return vehiculo;
}

//----------------------------------------------------------------------------------

// Obtener datos de usuario----------------------------------------------------------

Usuario infUsuario()
{

    Usuario usuario;

    cin.ignore();

    cout << "Nombre del usuario: ";
    cin.getline(usuario.nombre, 45, '\n');

    cout << "Sector actual en que se encuentra el usuario: " << endl;
    mostrarSectores();
    cout << "Ingrese el numero del sector -> ";
    cin >> usuario.sectorActual;

    cout << "Cédula del usuario: ";
    cin >> usuario.ci;

    return usuario;
}

// Función para desocupar vehiculo

void desocuparVehiculo(int ci)
{
    DB.open("vehiculos.dat", ios::in | ios::out | ios::binary);

    if (DB.is_open())
    {
        while (DB.read((char *)&vehiculo, sizeof(Vehiculo)))
        {
            if (vehiculo.ci == ci)
            {
                vehiculo.ocupado = false;
                DB.seekp(-static_cast<int>(sizeof(Vehiculo)), ios::cur);
                DB.write((char *)&vehiculo, sizeof(Vehiculo));
                break;
            }
        }
    }
    DB.close();
}

//----------------------------------------------------------------------------------

void mostrarSectores()
{

    ifstream sectores("sectores.txt");

    if (!sectores.is_open())
    {
        cout << "Fallo al abrir el archivo" << endl;
        return;
    }

    string linea;
    int id;
    string nombreSector;

    while (getline(sectores, linea))
    {

        stringstream ss(linea);
        char coma;

        ss >> id >> coma >> nombreSector;

        cout << id << ") " << nombreSector << endl;
    }

    sectores.close();
}

string mostrarSector(int idObjetivo)
{

    ifstream sectores("sectores.txt");

    string linea;
    int idSector;
    string nombreSector;
    string resultado;

    while (getline(sectores, linea))
    {

        stringstream ss(linea);
        char coma;

        ss >> idSector >> coma >> nombreSector;

        if (idSector == idObjetivo)
        {
            resultado = nombreSector;
        }
    }

    return resultado;
}

void leerDistancias()
{
    ifstream archivo("distancias.txt");
    if (!archivo.is_open())
    {
        cerr << "Error al abrir el archivo." << endl;
        return;
    }

    string linea;
    int origen, destino, peso;

    while (getline(archivo, linea))
    {
        stringstream ss(linea);
        char coma;

        ss >> origen >> coma >> destino >> coma >> peso;

        // Agregar nodos únicos
        if (find(nodos.begin(), nodos.end(), origen) == nodos.end())
            nodos.push_back(origen);
        if (find(nodos.begin(), nodos.end(), destino) == nodos.end())
            nodos.push_back(destino);
    }

    archivo.close();
}

void inicializarGrafo()
{
    int n = nodos.size();

    sort(nodos.begin(), nodos.end());

    for (int i = 0; i < n; i++)
    {
        nodoIndice[nodos[i]] = i;
    }

    grafo.assign(n, vector<int>(n, INF));

    for (int i = 0; i < n; i++)
    {
        grafo[i][i] = -1;
    }
}

void llenarGrafo()
{
    ifstream archivo("distancias.txt");
    if (!archivo.is_open())
    {
        cerr << "Error al abrir el archivo." << endl;
        return;
    }

    string linea;
    int origen, destino, peso;

    while (getline(archivo, linea))
    {
        stringstream ss(linea);
        char coma;
        ss >> origen >> coma >> destino >> coma >> peso;

        int i = nodoIndice[origen];
        int j = nodoIndice[destino];

        grafo[i][j] = peso;
        grafo[j][i] = peso; // Grafo no dirigido
    }

    archivo.close();
}

void mostrarMatriz()
{
    cout << "\n--- Matriz de Adyacencia ---\n   ";
    for (int nodo : nodos)
        cout << nodo << " ";
    cout << endl;

    for (size_t i = 0; i < grafo.size(); i++)
    {
        cout << nodos[i] << " ";
        for (size_t j = 0; j < grafo[i].size(); j++)
        {
            cout << grafo[i][j] << " ";
        }
        cout << endl;
    }
}

void encontrarCamino(int sectorOrigen, int sectorDestino)
{
    ocupados.clear();
    ruta.clear();
    pesoTotal = 0;

    cout << "sector de origen: " << sectorOrigen << endl;
    cout << "sector de destino: " << sectorDestino << endl;

    if (nodoIndice.find(sectorOrigen) == nodoIndice.end() || nodoIndice.find(sectorDestino) == nodoIndice.end())
    {
        cout << "Uno de los nodos no existe en el grafo.\n";
        return;
    }

    int actual = nodoIndice[sectorOrigen];

    while (true)
    {
        ocupados.push_back(actual);

        int minPeso = INF;
        int siguienteNodo = -1;

        int start = (sectorDestino > sectorOrigen) ? 0 : nodos.size() - 1;
        int end = (sectorDestino > sectorOrigen) ? nodos.size() : -1;
        int step = (sectorDestino > sectorOrigen) ? 1 : -1;

        for (int j = start; j != end; j += step)
        {
            if (grafo[actual][j] != INF && find(ocupados.begin(), ocupados.end(), j) == ocupados.end())
            {
                if (minPeso == INF || grafo[actual][j] < minPeso)
                {
                    minPeso = grafo[actual][j];
                    siguienteNodo = j;
                }
            }
        }

        if (siguienteNodo == -1)
        {
            cout << "No hay ruta disponible.\n";
            return;
        }

        ruta.push_back({nodos[actual], minPeso});
        pesoTotal += minPeso;

        if (nodos[siguienteNodo] == sectorDestino)
        {
            ruta.push_back({sectorDestino, 0});
            break;
        }

        actual = siguienteNodo;
    }

    cout << "\n--- recorrido a su destino ---\n"
         << endl;
    for (size_t i = 0; i < ruta.size() - 1; i++)
    {
        cout << "[" << ruta[i].first << "]--(" << ruta[i].second << ")--";
    }
    cout << "[" << ruta.back().first << "]" << endl;
    cout << "Recorrido de " << pesoTotal << " km aprox.\n";
}

void cargarGrafo()
{
    leerDistancias();
    inicializarGrafo();
    llenarGrafo();
    mostrarMatriz();
}

int obtenerId()
{

    ifstream sectores("sectores.txt");

    string linea;
    int id;

    while (getline(sectores, linea))
    {

        stringstream ss(linea);
        char coma;

        ss >> id >> coma;
    }
    sectores.close();
    return id + 1;
}

void reemplazarEspacios(char *texto)
{
    for (int i = 0; texto[i] != '\0'; i++)
    {
        if (texto[i] == ' ')
        {
            texto[i] = '_';
        }
    }
}

void restaurarEspacios(char *texto)
{
    for (int i = 0; texto[i] != '\0'; i++)
    {
        if (texto[i] == '_')
        {
            texto[i] = ' ';
        }
    }
}