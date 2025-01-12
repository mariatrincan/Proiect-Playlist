#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<exception>
using namespace std;

class Eroare : public exception {

public:
    virtual const char* what() const throw() {
        return "Eroare";
    }
};


class Muzica {
protected:
    string titlu;
    string artist;
    int durata;

public:
    Muzica():titlu(""), artist(""), durata(0) {}
    Muzica(string titlu, string artist, int durata): titlu(titlu), artist(artist), durata(durata) {}

     Muzica(const Muzica& m): titlu(m.titlu), artist(m.artist), durata(m.durata) {}

     Muzica& operator=(const Muzica& m) {
        if (this != &m) {
            titlu = m.titlu;
            artist = m.artist;
            durata = m.durata;
        }
        return *this;
    }

    Muzica operator+(const Muzica& m) const {
        string newtitlu = titlu + "+" + m.titlu;
        int newdurata= durata + m.durata;
        return Muzica(newtitlu,"Colaborare", newdurata);
    }

     virtual ~Muzica() {}

    friend bool operator==(const Muzica& m1, const Muzica& m2) {
        return m1.durata == m2.durata;
    }

    string get_titlu(){return titlu;}
    string get_artist(){return artist;}
    int get_durata(){return durata;}

    virtual ostream& print(ostream& os) const {
        os <<"Titlu: "<<titlu <<endl<<"Artist: "<<artist <<endl<<"Durata: "<<durata<<" de secunde"<<endl;
        return os;
    }
    friend ostream& operator<<(ostream& os, const Muzica& m) {
       return m.print(os);
    }

    friend istream& operator>>(istream& is, Muzica& m) {
        cout<<"Titlu: ";
        is>>m.titlu;
        cout<<"Artist: ";
        is>>m.artist;
        cout<<"Durata(in secunde): ";
        is>>m.durata;
        return is;
    }

};

class Muzica_Rock : public Muzica {
private:
    int solo;
public:
    Muzica_Rock(string titlu, string artist, int durata, int solo): Muzica(titlu,artist,durata), solo(solo) {}
    int get_solo() const { return solo; }

    Muzica_Rock(const Muzica_Rock& m) : Muzica(m), solo(m.solo) {}

     Muzica_Rock& operator=(const Muzica_Rock& m) {
        if (this != &m) {
            Muzica::operator=(m);
            solo = m.solo;
        }
        return *this;
    }

    virtual ~Muzica_Rock() {}


    ostream& print(ostream& os) const override {
        Muzica::print(os);
        os<<"Solo: "<<(solo ? "Da" : "Nu")<<endl;
        return os;
    }

    friend istream& operator>>(istream& is, Muzica_Rock& m) {
        is>>static_cast<Muzica&>(m);
        cout<<"Solo(1 pentru da, 0 pentru nu): ";
        is>>m.solo;
        return is;
    }
};


class Muzica_Pop : public Muzica {
private:
    int popularitate;
public:
    Muzica_Pop(string titlu, string artist, int durata, int popularitate): Muzica(titlu,artist,durata), popularitate(popularitate) {}
    int get_popularitate() const { return popularitate; }
   //friend ostream& operator<<(ostream& os, const Muzica_Pop& m)

    Muzica_Pop(const Muzica_Pop& m): Muzica(m), popularitate(m.popularitate) {}

    Muzica_Pop& operator=(const Muzica_Pop& m) {
        if (this != &m) {
            Muzica::operator=(m);
            popularitate = m.popularitate;
        }
        return *this;
    }

    virtual ~Muzica_Pop() {}

     ostream& print(ostream& os) const override{
        Muzica::print(os);
        os<<"Popularitate: "<<popularitate<<endl;
        return os;
    }
    friend istream& operator>>(istream& is, Muzica_Pop& m) {
        is>>static_cast<Muzica&>(m);
        cout<<"Popularitate(1-10): ";
        is>>m.popularitate;
        return is;
    }
};

class Playlist {
private:
    vector<Muzica*> melodii;
public:
    vector <Muzica*> get_melodii() const { return melodii; }
    void adaugare_melodie() {
        int tip_melodie;
        cout<<"Alege genul melodiei pe care vrei sa il adaugi: \n 1.Rock\n 2.Pop"<<endl;
        cin>>tip_melodie;

        Muzica* melodie=nullptr;

        if(tip_melodie==1) {
            melodie= new Muzica_Rock("","",0,0);
            cin>>*static_cast<Muzica_Rock*>(melodie);
        }
        else if(tip_melodie==2) {
            melodie= new Muzica_Pop("","",0,0);
            cin>>*static_cast<Muzica_Pop*>(melodie);
        }
        else {
            throw Eroare();
        }
        melodii.push_back(melodie);
    }

    void afisare() const {
        if (melodii.empty()) {
            throw Eroare();
        }

        for (const auto& melodie : melodii) {
            cout<< *melodie <<endl;
        }
    }

    void sortare() {
        sort(melodii.begin(), melodii.end(), [](Muzica* m1, Muzica* m2) {
            if (m1->get_durata()==m2->get_durata()) {
                return m1->get_titlu()<m2->get_titlu();
            }
            return m1->get_durata() < m2->get_durata();
        });
    }

   ~Playlist() {
        for (const auto& melodie : melodii) {
            delete melodie;
        }
    }
};

class Meniu_Interactiv {
    private:
    Playlist playlist;
    public:
      void run() {
          int optiune=0;
          try{
              while(optiune!=4) {
                  cout<<"\nMeniu:"<<endl;
                  cout<<"1.Adauga o melodie: "<<endl;
                  cout<<"2.Afiseaza melodiile din playlist: "<<endl;
                  cout<<"3.Sorteaza melodiile din playlist dupa durata: "<<endl;
                  cout<<"4.Combina 2 melodii"<<endl;
                  cout<<"5.Compara 2 melodii"<<endl;
                  cout<<"6.Iesire din aplicatie"<<endl;
                  cout<<"Alege o optiune: ";
                  cin>>optiune;

                  switch(optiune) {
                      case 1:
                          playlist.adaugare_melodie();
                      break;
                      case 2:
                          playlist.afisare();
                      break;
                      case 3:
                          playlist.sortare();
                      break;
                      case 4: {
                          int in1, in2;
                          cout<<"Alege prima melodie: "<<endl;
                          cin>>in1;
                          cout<<"Alege a doua melodie: "<<endl;
                          cin>>in2;

                          if (in1>=0 && in1< playlist.get_melodii().size() && in2>=0 && in2< playlist.get_melodii().size()) {
                              Muzica* m1= playlist.get_melodii()[in1];
                              Muzica* m2= playlist.get_melodii()[in2];

                              Muzica combinare= *m1 + *m2;
                              cout<<"Melodia combinata este: "<<endl;
                              cout<<combinare<<endl;
                          } else {
                              cout<<"Indicele este invalid!";
                              throw Eroare();
                          }
                          break;
                      }
                      case 5: {
                          int in1, in2;
                          cout<<"Alege prima melodie: "<<endl;
                          cin>>in1;
                          cout<<"Alege a doua melodie: "<<endl;
                          cin>>in2;

                          if (in1>=0 && in1< playlist.get_melodii().size() && in2>=0 && in2< playlist.get_melodii().size()) {
                              Muzica* m1= playlist.get_melodii()[in1];
                              Muzica* m2= playlist.get_melodii()[in2];

                              if (*m1==*m2) {
                                  cout<<"Melodiile au aceeasi durata!"<<endl;
                              }
                              else {
                                  cout<<"Melodiile au durate diferite!"<<endl;
                              }
                          }
                              else {
                                  cout<<"Indicele este invalid!";
                                  throw Eroare();
                              }
                          break;
                          }
                      case 6:
                              cout<<"Iesire din aplicatie"<<endl;
                          break;
                      default:
                              throw Eroare();
                      }
                  }

              }catch (const Eroare& e) {
                  cout<<e.what()<<endl;
              }
          }


};

int main() {
    Meniu_Interactiv meniu;
    meniu.run();
    return 0;
}