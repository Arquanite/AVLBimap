#include <iostream>
#include <string>
#include "UlepszeniaTerminala.h"
#include "AVLBiMap.h"
using namespace std;

void WypiszMenu(vector<string> tekst, vector<string> naglowek, int aktywne = 0){
    UstawKolor();
    Wyczysc();
    UstawKolor(Zielony);
    for(int i=0; i<naglowek.size(); i++){
        UstawKursor((RozmiarKonsoli().x-80)/2,i);
        cout<<naglowek[i];
    }
    for(int i=0; i<tekst.size(); i++){
        UstawKursor((RozmiarKonsoli().x-tekst[i].size())/2-1,i+(RozmiarKonsoli().y-tekst.size())/2+2);
        if(i == aktywne) UstawKolor(Czarny, Bialy);
        else UstawKolor(Bialy, Czarny);
        cout<<i+1<<": "<<tekst[i];
    }
}

int main(){
    AVLTree<string> Drzewo;
    AVLBiMap<string,string> Slownik;

    vector<string> Naglowek = {"██████╗ ██████╗ ███████╗███████╗██╗    ██╗ ██████╗      █████╗ ██╗   ██╗██╗     ",
                               "██╔══██╗██╔══██╗╚══███╔╝██╔════╝██║    ██║██╔═══██╗    ██╔══██╗██║   ██║██║     ",
                               "██║  ██║██████╔╝  ███╔╝ █████╗  ██║ █╗ ██║██║   ██║    ███████║██║   ██║██║     ",
                               "██║  ██║██╔══██╗ ███╔╝  ██╔══╝  ██║███╗██║██║   ██║    ██╔══██║╚██╗ ██╔╝██║     ",
                               "██████╔╝██║  ██║███████╗███████╗╚███╔███╔╝╚██████╔╝    ██║  ██║ ╚████╔╝ ███████╗",
                               "╚═════╝ ╚═╝  ╚═╝╚══════╝╚══════╝ ╚══╝╚══╝  ╚═════╝     ╚═╝  ╚═╝  ╚═══╝  ╚══════╝"};
    vector<string> Menu = {"Znajdź nowe słowa w tekście",
                           "Wczytaj słowa wraz z tłumaczeniami",
                           "Dodaj nowe słowo",
                           "Usuń słowo (angielskie)",
                           "Usuń słowo (polskie)",
                           "Znajdź tłumaczenie angielski-polski",
                           "Znajdź tłumaczenie polski-angielski",
                           "Pokaż drzewo (do deduplikacji)",
                           "Pokaż drzewo (angielskie)",
                           "Pokaż drzewo (polskie)",
                           "Wypisz wyrazy i tłumaczenia (KLP)",
                           "Wyjdz z programu"};
    UstawKolor();
    Wyczysc();
    WypiszMenu(Menu, Naglowek, 0);
    ZmienTryb(true);
    int wybor = 0;

    while(true){
        switch (Znak()) {
        case 'w':
            wybor--;
            break;
        case 's':
            wybor++;
            break;
        case 'q':
            UstawKolor();
            Wyczysc();
            UstawKursor((RozmiarKonsoli().x-40)/2, RozmiarKonsoli().y/2-2);
            return 0;
        case Enter:
            switch (wybor) {
            case 0:
                Wyczysc();
                UstawKolor();
                UstawKursor(0,0);
                cout<<"Czytam z pliku ";
                UstawKolor(Czerwony);
                cout<<"In0502.txt";
                UstawKolor();
                cout<<":"<<endl;
                if(true){
                    ifstream in("In0502.txt");
                    vector<string> values;
                    if(!in.good()){
                        UstawKolor(Czarny, Czerwony);
                        cout<<"Błąd odczytu pliku wejściowego!"<<endl;
                    }
                    else {
                        while(!in.eof()){
                            string temp;
                            in>>temp;
                            cout<<temp;
                            if(temp.empty()) continue;
                            if(Drzewo.Exists(temp)) cout<<" - duplikat"<<endl;
                            else {
                                cout<<endl;
                                UstawKolor(Niebieski);
                                Drzewo.Insert(temp);
                                values.push_back(temp);
                                UstawKolor();
                            }
                            Poczekaj(100);
                        }
                    }
                    in.close();

                    ofstream out("OutA0502.txt");
                    cout<<endl<<"Zapis do pliku ";
                    UstawKolor(Czerwony);
                    cout<<"OutA0502.txt";
                    UstawKolor();
                    cout<<"..."<<endl;
                    for(string v : values){
                        cout<<v<<" ";
                        out<<v<<" ";
                    }
                    cout<<endl<<endl;
                    out.flush();
                    out.close();
                    Poczekaj(500);
                }
                UstawKolor();
                cout<<"Naciśnij dowolny klawisz aby kontynuować..."<<endl;
                Znak();
                break;
            case 1:
                Wyczysc();
                UstawKolor();
                UstawKursor(0,0);
                cout<<"Czytam z pliku ";
                UstawKolor(Czerwony);
                cout<<"OutAa0502.txt";
                UstawKolor();
                cout<<":"<<endl;
                if(true){
                    ifstream in("OutAa0502.txt");
                    if(!in.good()){
                        UstawKolor(Czarny, Czerwony);
                        cout<<"Błąd odczytu pliku wejściowego!"<<endl;
                    }
                    else {
                        while(!in.eof()){
                            string temp1, temp2;
                            in>>temp1>>temp2;
                            if(temp1.empty() || temp2.empty()) continue;
                            cout<<temp1<<'-'<<temp2;
                            if(Slownik.ExistsByKey(temp1) || Slownik.ExistsByValue(temp2)) cout<<" - duplikat"<<endl;
                            else {
                                cout<<endl;
                                UstawKolor(Niebieski);
                                Slownik.Insert(temp1, temp2);
                                UstawKolor();
                            }
                            Poczekaj(100);
                        }
                    }
                    in.close();
                    Poczekaj(500);
                }
                cout<<endl;
                UstawKolor();
                cout<<"Naciśnij dowolny klawisz aby kontynuować..."<<endl;
                Znak();
                break;
            case 2:
                Wyczysc();
                UstawKolor();
                UstawKursor(0,0);
                ZmienTryb(false);
                if(true){
                    string temp1, temp2;
                    cout<<"Angielski: ";
                    cin>>temp1;
                    if(!Slownik.ExistsByKey(temp1)){
                        cout<<"Polski: ";
                        cin>>temp2;
                        if(!Slownik.ExistsByValue(temp2)){
                            UstawKolor(Niebieski);
                            Slownik.Insert(temp1,temp2);
                            UstawKolor();
                            cout<<"Dodano pomyślnie"<<endl;
                        }
                        else {
                            UstawKolor();
                            cout<<"Wystąpił błąd, słowo ";
                            UstawKolor(Czerwony);
                            cout<<temp2;
                            UstawKolor();
                            cout<<" zostało już dodane do słownika."<<endl;
                            cout<<"Jeżeli chcesz zmienić tłumaczenie, usuń je i dodaj ponownie"<<endl;
                        }
                    }
                    else {
                        UstawKolor();
                        cout<<"Wystąpił błąd, słowo ";
                        UstawKolor(Czerwony);
                        cout<<temp1;
                        UstawKolor();
                        cout<<" zostało już dodane do słownika"<<endl;
                        cout<<"Jeżeli chcesz zmienić tłumaczenie, usuń je i dodaj ponownie"<<endl;
                    }
                }
                Poczekaj(500);
                cout<<endl;
                UstawKolor();
                cout<<"Naciśnij dowolny klawisz aby kontynuować..."<<endl;
                cin.ignore();
                ZmienTryb(true);
                Znak();
                break;
            case 3:
                Wyczysc();
                UstawKolor();
                UstawKursor(0,0);
                ZmienTryb(false);
                if(true){
                    string temp;
                    cout<<"Słowo do usunięcia: ";
                    cin>>temp;
                    if(Slownik.ExistsByKey(temp)){
                        UstawKolor(Niebieski);
                        Slownik.DeleteByKey(temp);
                        UstawKolor();
                        cout<<"Usunięto pomyślnie"<<endl;
                    }
                    else {
                        UstawKolor();
                        cout<<"Wystąpił błąd, słowo ";
                        UstawKolor(Czerwony);
                        cout<<temp;
                        UstawKolor();
                        cout<<" nie występuje w słowniku angielskim"<<endl;
                    }
                }
                Poczekaj(500);
                cout<<endl;
                UstawKolor();
                cout<<"Naciśnij dowolny klawisz aby kontynuować..."<<endl;
                cin.ignore();
                ZmienTryb(true);
                Znak();
                break;
            case 4:
                Wyczysc();
                UstawKolor();
                UstawKursor(0,0);
                ZmienTryb(false);
                if(true){
                    string temp;
                    cout<<"Słowo do usunięcia: ";
                    cin>>temp;
                    if(Slownik.ExistsByValue(temp)){
                        UstawKolor(Niebieski);
                        Slownik.DeleteByValue(temp);
                        UstawKolor();
                        cout<<"Usunięto pomyślnie"<<endl;
                    }
                    else {
                        UstawKolor();
                        cout<<"Wystąpił błąd, słowo ";
                        UstawKolor(Czerwony);
                        cout<<temp;
                        UstawKolor();
                        cout<<" nie występuje w słowniku polskim"<<endl;
                    }
                }
                Poczekaj(500);
                cout<<endl;
                UstawKolor();
                cout<<"Naciśnij dowolny klawisz aby kontynuować..."<<endl;
                cin.ignore();
                ZmienTryb(true);
                Znak();
                break;
            case 5:
                Wyczysc();
                UstawKolor();
                UstawKursor(0,0);
                ZmienTryb(false);
                if(true){
                    string temp;
                    cout<<"Angielski: ";
                    cin>>temp;
                    if(Slownik.ExistsByKey(temp)){
                        cout<<"Polski: "<<Slownik.Value(temp)<<endl;
                    }
                    else {
                        UstawKolor();
                        cout<<"Wystąpił błąd, słowo ";
                        UstawKolor(Czerwony);
                        cout<<temp;
                        UstawKolor();
                        cout<<" nie występuje w słowniku angielskim"<<endl;
                    }
                }
                Poczekaj(500);
                cout<<endl;
                UstawKolor();
                cout<<"Naciśnij dowolny klawisz aby kontynuować..."<<endl;
                cin.ignore();
                ZmienTryb(true);
                Znak();
                break;
            case 6:
                Wyczysc();
                UstawKolor();
                UstawKursor(0,0);
                ZmienTryb(false);
                if(true){
                    string temp;
                    cout<<"Polski: ";
                    cin>>temp;
                    if(Slownik.ExistsByValue(temp)){
                        cout<<"Angielski: "<<Slownik.Key(temp)<<endl;
                    }
                    else {
                        UstawKolor();
                        cout<<"Wystąpił błąd, słowo ";
                        UstawKolor(Czerwony);
                        cout<<temp;
                        UstawKolor();
                        cout<<" nie występuje w słowniku polskim"<<endl;
                    }
                }
                Poczekaj(500);
                cout<<endl;
                UstawKolor();
                cout<<"Naciśnij dowolny klawisz aby kontynuować..."<<endl;
                cin.ignore();
                ZmienTryb(true);
                Znak();
                break;
            case 7:
                Drzewo.Print();
                break;
            case 8:
                Slownik.PrintByKey();
                break;
            case 9:
                Slownik.PrintByValue();
                break;
            case 10:
                Wyczysc();
                UstawKolor();
                UstawKursor(0,0);
                cout<<"Tłumaczenia angielski-polski w porządku KLP:"<<endl;
                if(true){
                    ofstream out("OutB0502.txt");
                    vector<string> words = Slownik.Keys();
                    for(string word : words){
                        cout<<word<<' '<<Slownik.Value(word)<<endl;
                        out<<word<<' '<<Slownik.Value(word)<<endl;
                        Poczekaj(100);
                    }
                    out.flush();
                    out.close();
                }
                cout<<endl<<"Zapis do pliku ";
                UstawKolor(Czerwony);
                cout<<"OutB0502.txt";
                UstawKolor();
                cout<<"..."<<endl;
                Poczekaj(500);
                cout<<endl;
                UstawKolor();
                cout<<"Naciśnij dowolny klawisz aby kontynuować..."<<endl;
                cin.ignore();
                Znak();
                break;
            case 11:
                UstawKolor();
                Wyczysc();
                UstawKursor((RozmiarKonsoli().x-40)/2, RozmiarKonsoli().y/2-2);
                return 0;
            }
            break;
        }
        if(wybor == Menu.size()) wybor = 0;
        else if(wybor == -1) wybor = Menu.size()-1;
        WypiszMenu(Menu, Naglowek, wybor);
    }

    return 0;
}
