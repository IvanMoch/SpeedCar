#include<iostream>
#include<string.h>
#include<fstream>

using namespace std;

// Declaración de las variables y estructuras que se implementaran en el proyecto------------------------
struct Vehiculo {
    char modelo[45];
    char conductor[45];
    char sectorActual[45];
    char placa[20];
    int edad;
    int ci;
    bool ocupado;
};

struct Usuario {
    int ci;
    char nombre[45];
    char sectorActual[45];
};

struct Sector {
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
bool verificarExistenciaSector(string nombre);
void mostrarVehiculosCerca(int ci);
void ubicacionVehiculo();
void solicitudTraslado(int ci);
Vehiculo infVehiculo();
Usuario infUsuario();
int ci;

fstream DB;


Usuario usuario;
Vehiculo vehiculo;
Sector sector;

//-------------------------------------------------------------------------------------





// Función main------------------------------------------------------------------------

int main(){

    int aux;

    do
    {
        string entidad;
        aux = MenuPrincipal();

        switch (aux)
        {
        case 1:
            subMenuGestion();
            break;
        case 2:
            subMenuServicio();
        default:
            break;
        }
    } while (aux != 0);

    cout << "Has salido de la aplicacion" << endl;

    return 0;
}

//-----------------------------------------------------------------------------------






//Menu principal---------------------------------------------------------------------

/*Esta función muestra por pantalla las opciones del menú principal y retorna un numero que sirve como indicador 
para indicar a que menú desea ir el usuario */

int MenuPrincipal(){
    system("clear");
    int aux;

    cout << "BIENVENIDO A SpeedCar"<<endl;
    cout << "1) Menu de gestion"<<endl;
    cout << "2) menu de servicios"<<endl;
    cout << "0) salir de la aplicacion" << endl;
    cout << "Ingrese su opcion ->";
    cin >> aux;

    return aux;
}

//-----------------------------------------------------------------------






//Sub menu del menu de gestión ------------------------------------------

/*Esta funcion muestra por pantalla las opciones que tiene el usuario para gestionar*/

void subMenuGestion(){ 
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

int subMenuEntidad( string entidad) {
    system("clear");
    int aux;

    if(!strcasecmp(entidad.c_str(), "Vehiculo") || !strcasecmp(entidad.c_str(), "Usuario")){
    cout << "1)Crear " << entidad << endl;
    cout << "2)Mostrar "<< entidad << endl;
    cout << "3)Modificar " << entidad << endl;
    cout << "4)Borrar "<< entidad << endl;
    cout << "5)Salir del menu" << endl;
    cout << "Ingrese su opción -> ";
    cin >> aux;
    cout << endl;
    }

    if(!strcasecmp(entidad.c_str(), "Sector")){
        cout << "1)Crear " << entidad << endl;
        cout << "4)Borrar " << entidad << endl;
        cout << "Ingrese su opción ->";
        cin >> aux;
    }

    return aux;
}

//----------------------------------------------------------------------------------






//Menu de servicios-----------------------------------------------------------------

/* Esta función muestra por pantalla las funciones que puede hacer el usuario en el menu servicio*/

void subMenuServicio(){
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

	switch(aux){
		case 1:ubicacionVehiculo();
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





//CRUD------------------------------------------------------------------------------

void crearEntidad(string entidad) {
    system("clear");

    if (!(strcasecmp(entidad.c_str(), "Vehiculo"))) {
        Vehiculo vehiculo = infVehiculo(); // Almacena los datos en la variable global
        if (verificarExistencia(entidad, vehiculo.ci)) {
            cout << "El usuario ingresado ya existe" << endl;
            system("read -p 'Presione enter para volver al menu principal...' var");
            DB.close();
            MenuPrincipal();
        } else {
            DB.open("vehiculos.dat", ios::out | ios::app | ios::binary);
            if (DB.is_open()) {
                DB.write((char *)&vehiculo, sizeof(Vehiculo));
                DB.close();
                system("clear");
                cout << "-------------------" << endl;
                cout << "|Vehiculo guardado|" << endl;
                cout << "-------------------" << endl;
                system("read -p 'presione enter para continuar' var");
            } else {
                cerr << "No se pudo abrir el archivo vehiculos.da" << endl;
            }
        }
    }

    if (!strcasecmp(entidad.c_str(), "Usuario")) {
        Usuario usuario = infUsuario(); // Almacena los datos en la variable global "usuario"
        if (verificarExistencia(entidad, usuario.ci)) {
            cout << "El usuario ingresado ya existe" << endl;
            system("read -p 'Presione enter para volver al menu principal...' var");
            DB.close();
            MenuPrincipal();
        } else {
            DB.open("usuarios.dat", ios::out | ios::app | ios::binary);
            if (DB.is_open()) {
                DB.write((char *)&usuario, sizeof(Usuario));
                DB.close();
                system("clear");
                cout << "------------------" << endl;
                cout << "|Usuario guardado|" << endl;
                cout << "------------------" << endl;
                system("read -p 'presione enter para continuar' var");
            } else {
                cerr << "No se pudo abrir el archivo usuarios.da" << endl;
            }
        }
    }

    if (!strcasecmp(entidad.c_str(), "Sector")){
        Sector sector;

        cin.ignore();
        cout << "Coloque el nombre del nuevo sector" << endl;
        cin.getline(sector.nombre, 100, '\n');

        if(verificarExistenciaSector(sector.nombre)){
            cout << "El sector ya existe" << endl;
            system("read -p 'Pulse enter para volver al menu principal' var");
            MenuPrincipal();
        }else{
            DB.open("Sectores.dat", ios::out | ios::app | ios::binary);

            if(DB.is_open()){
                DB.write((char *)&sector, sizeof(Sector));
                DB.close();
                system("clear");
                cout << "-----------------" << endl;
                cout << "|Sector guardado|" << endl;
                cout << "-----------------" << endl;
                system("read -p 'presione enter para continuar' var");

            }
        }
    }
}

//-------------------------------------------------------------------------------

void mostrarEntidad(string entidad){
    system("clear");
    int ci;

    if (!strcasecmp(entidad.c_str(), "vehiculo")){
        /* Codigo para mostrar un vehiculo en vehiculos.da */

        cout << "CI del conductor que busca: ";
        cin >> ci;

        DB.open("vehiculos.dat", ios::in | ios::binary);

        if(DB.is_open()){
            while (DB.read((char*)&vehiculo, sizeof(Vehiculo)))
            {
                if(vehiculo.ci == ci){
                    cout << "----------Información del conductor----------" << endl;
                    cout << "|nombre del conductor: " << vehiculo.conductor << "|" << endl;
                    cout << "|Modelo del vehiculo: " << vehiculo.modelo << "|" << endl;
                    cout <<"|Placa del vehiculo: " << vehiculo.placa << "  |" << endl;
                    cout << "|Sector actual: " << vehiculo.sectorActual << "|" << endl;

                    if(vehiculo.ocupado){
                        cout << "|El conductor se encuentra haciendo una carrera|" << endl;
                    }else{
                        cout << "|El conductor esta libre para hacer una carrera|" << endl;
                    }

                    cout << "---------------------------------------------" << endl;
                    
                    break;
            }
            
        }
        system("read -p 'presione enter para continuar' var");
    }
}
    if (!strcasecmp(entidad.c_str(),"usuario")){

        cout << "CI del usuario que busca: ";
        cin >> ci;

        DB.open("usuarios.dat", ios::in | ios::binary);

        if(DB.is_open()){
            while (DB.read((char*)&usuario, sizeof(Usuario)))
            {
                if(usuario.ci == ci){
                    cout << "----------Informacion del usuario----------" << endl;
                    cout << "|nombre del conductor: " << usuario.nombre << "|" << endl;
                    cout << "|Sector actual: " << usuario.sectorActual << "|" << endl;
                    cout << "---------------------------------------------" << endl;
                    break;
            }
            
        }
        system("read -p 'presione enter para continuar' var");
    }   
    }
    if (!strcasecmp(entidad.c_str(),"sector")){
        /* Código para mostrar un sector en sectores.da */
    }

    DB.close();
}

//--------------------------------------------------------------------

void modificarEntidad(string entidad){
    system("clear");

    if (!strcasecmp(entidad.c_str(),"vehiculo")){
        /* Codigo para ingresar un vehiculo en vehículos.da */
        bool encontrado=false;
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
                cin.getline(vehiculo.sectorActual, 45, '\n');
                cout << "Edad: ";
                cin >> vehiculo.edad;
                
                
                DB.seekp(-static_cast<int>(sizeof(Vehiculo)), ios::cur); // Metodo para regresar en el archivo para sobreescribir en el archivo (Indagar)
                DB.write((char *)&vehiculo, sizeof(Vehiculo));

                encontrado = true;
                break;
            }
	}
	
	if(!encontrado){
		cout<<"Vehiculo no encontrado" << endl;
	}
	DB.close();
    }


    if (!strcasecmp(entidad.c_str(),"usuario")){
        bool encontrado=false;
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
                cin.getline(usuario.sectorActual, 45, '\n');
                
                
                DB.seekp(-static_cast<int>(sizeof(Usuario)), ios::cur); 
                DB.write((char *)&usuario, sizeof(Usuario));

                encontrado = true;
                break;
            }
	}
	
	if(!encontrado){
		cout<<"Usuario no encontrado" << endl;
	}
	DB.close();
    }
    
}

//------------------------------------------------------------------------

void BorrarEntidad(string entidad){
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

                memset(vehiculo.conductor,'\0',sizeof(vehiculo.conductor));
			    vehiculo.edad=0;
                vehiculo.ci=0;

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
    
    if (!strcasecmp(entidad.c_str(),"usuario")){

        bool encontrado = false;

        cin.ignore();
        cout << "Introduzca la cedula del usuario: ";
        cin >> ci;

        DB.open("usuarios.dat", ios::in | ios::out | ios::binary);

        while (DB.read((char *)&usuario, sizeof(Usuario)))
        {
            if (usuario.ci == ci)
            {

                memset(usuario.nombre,'\0',sizeof(usuario.nombre)); 
                usuario.ci=0;

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

    if (! strcasecmp(entidad.c_str(), "Sector")){
        bool encontrado = false;

        cin.ignore();
        cout << "Introduzca el nombre del sector: ";
        cin.getline(nombre, 100 , '\n');

        DB.open("Sectores.dat", ios::in | ios::out | ios::binary);

        while (DB.read((char *)&sector, sizeof(Sector)))
        {
            if (!strcasecmp(sector.nombre , nombre))
            {

                memset(sector.nombre,'\0',sizeof(sector.nombre));

                DB.seekp(-static_cast<int>(sizeof(Sector)), ios::cur);  
                DB.write(reinterpret_cast<char *>(&sector), sizeof(Sector));

                encontrado = true;
                cout << "Sector eliminado exitosamente" << endl;
                break;
            }
        }

        if (!encontrado)
        {
            cout << "Sector no encontrado" << endl;
        }
        DB.close();
    }
}

//-----------------------------------------------------------------------

bool verificarExistencia(string entidad, int ci){

    bool existe = false;

    if (!strcasecmp(entidad.c_str(),"vehiculo")){
        /* Codigo para ingresar un vehiculo en vehículos.da */
        DB.open("vehiculos.dat", ios::in | ios::binary);

        while (DB.read((char*)&vehiculo, sizeof(Vehiculo)))
        {
            if (vehiculo.ci == ci)
            {
                existe = true;
            }
        }
        
    }
    if (!strcasecmp(entidad.c_str(),"usuario")){
        DB.open("usuarios.dat", ios::in | ios::binary);

        while (DB.read((char*)&usuario, sizeof(Usuario)))
        {
            if (usuario.ci == ci)
            {
                existe = true;
            }
        }
        
    }
    
    DB.close();
    return existe;
}

bool verificarExistenciaSector( string nombre){
    
    bool existe = false;
    DB.open("Sectores.dat", ios::in | ios::binary);

    if(DB.is_open()){
    while (DB.read((char *)&sector,sizeof(Sector)))
        {
            cout << sector.nombre << " " << nombre << endl;
            system("read -p '...' var");
            if (!strcasecmp(sector.nombre, nombre.c_str()))
            {
                existe = true;
                break;
            }
        }
        
    }
    DB.close();
    return existe;
}

void ubicacionVehiculo(){
	system("clear");
	char placa[15];
    char nuevoSector[45];
    bool encontrado = false;

    cout << "Ingrese la placa del vehiculo: ";
    cin.ignore();
    cin.getline(placa, 15,'\n');

    

    DB.open("vehiculos.dat", ios::in | ios::out | ios::binary);

    while (DB.read(reinterpret_cast<char*>(&vehiculo), sizeof(Vehiculo))) {
        if (strcmp(vehiculo.placa, placa) == 0) {
        	cout<<"\tPlaca encontrada"<<endl;
        	cout<<"Ubicacion actual: "<<vehiculo.sectorActual<<endl;
        	cout << "Ingrese el nuevo sector del vehiculo: ";
    		cin.getline(nuevoSector, 45,'\n');
            strcpy(vehiculo.sectorActual, nuevoSector);

            DB.seekp(-static_cast<int>(sizeof(Vehiculo)), ios::cur);
            DB.write(reinterpret_cast<char*>(&vehiculo), sizeof(Vehiculo));

            encontrado = true;
            cout << "Ubicacion del vehiculo actualizada exitosamente." << endl;
            break;
        }
    }

    if (!encontrado) {
        cout << "Vehiculo no encontrado." << endl;
    }

    DB.close();
}

void solicitudTraslado(int ci) {
    cout << "---------------------------" << endl;
    char destino[30];
    bool realizado = false;

    DB.open("usuarios.dat", ios::in | ios::out | ios::binary);
    Usuario usuario;
    Vehiculo vehiculo;

    while (DB.read((char *)&usuario, sizeof(Usuario))) {
        if (usuario.ci == ci) {
            // Encontrado el usuario
            DB.close();  // Cerrar el archivo de usuarios antes de abrir el de veh\EDculos

            DB.open("vehiculos.dat", ios::in | ios::out | ios::binary);
            

            while (DB.read((char *)&vehiculo, sizeof(Vehiculo))) {
                if (!vehiculo.ocupado && !strcmp(vehiculo.sectorActual, usuario.sectorActual)) {
                    cout << "\tServicio Disponible en tu zona" << endl;
                    cout << "Sector de Origen: " << vehiculo.sectorActual << endl;
                    cout << "Ingrese el Destino: ";
                    cin.ignore();  // Limpiar el buffer antes de getline
                    cin.getline(destino, 100, '\n');

                    strcpy(vehiculo.sectorActual, destino);
                    strcpy(usuario.sectorActual, destino);

                    // Mover el puntero hacia atr\E1s para sobrescribir
                    DB.seekp(-static_cast<int>(sizeof(Vehiculo)), ios::cur);
                    DB.write(reinterpret_cast<char*>(&vehiculo), sizeof(Vehiculo));

                    realizado = true;
                    break;
                }
            }

            DB.close();  // Cerrar el archivo de veh\EDculos despu\E9s de la operaci\F3n
        }
    }

    if (realizado == 1) {
        cout << "----------REALIZANDO TRASLADO----------" << endl;
        system("read -p 'presione enter cuando termine el traslado' var");
        DB.open("usuarios.dat", ios::in | ios::out | ios::binary);
        if(DB.is_open()){
            while (DB.read((char *)&usuario, sizeof(Usuario)))
            {
                if (usuario.ci == ci)
                {
                    strcpy(usuario.sectorActual, destino);
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

void solicitarConductor(int ciUsuario, int ciConductor){
    cout << "---------------------------" << endl;
    char destino[30];
    bool realizado = false;

    DB.open("usuarios.dat", ios::in | ios::out | ios::binary);
    Usuario usuario;
    Vehiculo vehiculo;

    while (DB.read((char *)&usuario, sizeof(Usuario))) {
        if (usuario.ci == ci) {
            // Encontrado el usuario
            DB.close();  // Cerrar el archivo de usuarios antes de abrir el de veh\EDculos

            DB.open("vehiculos.dat", ios::in | ios::out | ios::binary);
            

            while (DB.read((char *)&vehiculo, sizeof(Vehiculo))) {
                if (!vehiculo.ocupado && !strcmp(vehiculo.sectorActual, usuario.sectorActual) && vehiculo.ci == ciConductor) {
                    cout << "\tServicio Disponible en tu zona" << endl;
                    cout << "Sector de Origen: " << vehiculo.sectorActual << endl;
                    cout << "Ingrese el Destino: ";
                    cin.ignore();  // Limpiar el buffer antes de getline
                    cin.getline(destino, 100, '\n');

                    strcpy(vehiculo.sectorActual, destino);
                    strcpy(usuario.sectorActual, destino);

                    // Mover el puntero hacia atr\E1s para sobrescribir
                    DB.seekp(-static_cast<int>(sizeof(Vehiculo)), ios::cur);
                    DB.write(reinterpret_cast<char*>(&vehiculo), sizeof(Vehiculo));

                    realizado = true;
                    break;
                }
            }

            DB.close();  // Cerrar el archivo de veh\EDculos despu\E9s de la operaci\F3n
        }
    }

    if (realizado == 1) {
        cout << "----------REALIZANDO TRASLADO----------" << endl;
        system("read -p 'presione enter cuando termine el traslado' var");
        DB.open("usuarios.dat", ios::in | ios::out | ios::binary);
        if(DB.is_open()){
            while (DB.read((char *)&usuario, sizeof(Usuario)))
            {
                if (usuario.ci == ci)
                {
                    strcpy(usuario.sectorActual, destino);
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

void mostrarVehiculosCerca(int ciUsuario){

    char sectorObjetivo[100];
    int ciConductor;
    bool usuarioEncontrado = false;
    bool vehiculoEncontrado = false;
    system("clear");

    DB.open("usuarios.dat", ios::in | ios::binary);

    if(DB.is_open()){
        while(DB.read((char*)&usuario, sizeof(Usuario))){
        if(usuario.ci == ciUsuario){
            strcpy(sectorObjetivo, usuario.sectorActual);
            usuarioEncontrado = true;
        }
    }
    }

    DB.close();

    DB.open("vehiculos.dat", ios::in | ios::binary);

    if(DB.is_open()){
        while (DB.read((char *)&vehiculo, sizeof(Vehiculo)))
        {
            if(!strcasecmp(vehiculo.sectorActual,sectorObjetivo) && !vehiculo.ocupado){
                    cout << "----------Información del conductor----------" << endl;
                    cout << "|Cédula del conductor: " << vehiculo.ci << "|" << endl;
                    cout << "|Nombre del conductor: " << vehiculo.conductor << "|" << endl;
                    cout << "|Modelo del vehiculo: " << vehiculo.modelo << "|" << endl;
                    cout <<"|Placa del vehiculo: " << vehiculo.placa << "  |" << endl;
                    cout << "|Sector actual: " << vehiculo.sectorActual << "|" << endl;
                    cout << "---------------------------------------------" << endl;
                    vehiculoEncontrado = true;
            }
        }
        
    }

    DB.close();

    if(vehiculoEncontrado){
        cin.ignore();
        cout << "Indique la cédula del conductor que desees : ";
        cin >> ciConductor;
        solicitarConductor(ciUsuario, ciConductor);
    }

}

//Obtener los datos del vehiculo---------------------------------------------------

Vehiculo infVehiculo() {
    // Resetear la estructura vehiculo
    Vehiculo vehiculo;

    cin.ignore();
    cout << "nombre del chofer: ";
    cin.getline(vehiculo.conductor, 45, '\n');
    cout << "modelo del vehiculo: ";
    cin.getline(vehiculo.modelo, 45, '\n');
    cout << "Placa del vehiculo: ";
    cin.getline(vehiculo.placa, 20, '\n');
    cout << "Sector actual en el que estas: ";
    cin.getline(vehiculo.sectorActual, 45, '\n');
    cout << "edad del conductor: ";
    cin >> vehiculo.edad;
    cout << "CI del conductor: ";
    cin >> vehiculo.ci;

    return vehiculo;
}

//----------------------------------------------------------------------------------

//Obtener datos de usuario----------------------------------------------------------

Usuario infUsuario(){

    Usuario usuario;

    cin.ignore();

    cout << "Nombre del usuario: ";
    cin.getline(usuario.nombre, 45, '\n');

    cout << "Sector actual en que se encuentra el usuario: ";
    cin.getline(usuario.sectorActual, 45, '\n');

    cout << "Cédula del usuario: ";
    cin >> usuario.ci;

    return usuario;
}

//----------------------------------------------------------------------------------