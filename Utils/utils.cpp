*/
Entrada: Path al archivo csv
Salida: Devuelve un vector de vector<double>  con el contenido de un archivo CSV
*/

vector<vector<double> > parse_csv(char *filename) {
        ifstream f;
        f.open(filename);
        
        if(!f.is_open()){
                cout<<"No se pudo abrir el archivo: "<<filename<<endl;
                getchar();
                exit(-1);
        }
        vector<vector<double> > X = vector<vector<double> >();
        double value;
        string s;
        //Lectura de los datos de entrenamiento
        while(getline(f,s)){
                stringstream ss(s);
                vector<double> tempy;
                while(ss>>value){
                        tempy.push_back(value);
                        if(ss.peek() == ',')
                                ss.ignore();
                }
                X.push_back(tempy);
        }
        return X;
}