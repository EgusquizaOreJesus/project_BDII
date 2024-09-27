//
// Created by paolo on 17/09/2024.
//

#include <iostream>
#include "tokenS.h"
#include "parserSQL.h"
#include "estructuras/avlFile.h"
using namespace std;

int main() {
    const char* input =
//            "CREATE TABLE Playstore FROM FILE 'data_playstore.csv' USING INDEX EXTENDIBLE('anime_id');";
    //            "INSERT INTO Animes VALUES (1, 'Naruto', 'Shounen', 'TV', 220, 8.3, 1000000);";
//            "INSERT INTO Youtube VALUES ('__4c1JCHv32', 'Mahou Shoujo Ep. 3', 'Aniplex', 3000000, 100000, 3512, 2000000);";
            "SELECT * FROM Playstore WHERE key = 'com.whatsopen.app';";
//            "SELECT * FROM Youtube WHERE key = '__4c1JCHvaQ';";
//            "SELECT * FROM Animes WHERE anime_id BETWEEN 1 AND 2;"
//            "DELETE FROM Youtube WHERE key = '__4c1JCHv32';";
//            "CREATE TABLE Apps FROM FILE 'app_data.csv' USING INDEX AVL('App_id');"
//            "INSERT INTO Apps VALUES ('com.example.app', 'Example App', 'Productivity', 4.5, 10000, '100K', 100000, 500000, true, 0.0, 'USD', '50MB');"
//            "SELECT * FROM Apps WHERE App_id = 'com.example.app';";
//

    Scanner scanner(input);

    const char* structure = "avlFileYoutube";
    structure = "extendibleFileYoutube";
    structure = "extendibleFilePlaystore";

    // int                                      -> para cargar data de playstore
    // const char*                              -> para cargar data de youtube
    Parser<int> parser(&scanner, structure);
    parser.parse();
//    cout << "get structure: " << parser.getStructure();


    return 0;
}
