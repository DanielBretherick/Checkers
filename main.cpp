//Jogo de Damas


#include <iostream>
#include <vector>

using namespace std;

enum cores {BRANCA, PRETA, NEUTRO};
enum tiposPeca {COMUM, RAINHA, VAZIO};
enum colunas {A, B, C, D, E, F, G, H};

vector<char> letrasColunas({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}) ;
char letraCores[3] = {'B', 'P'};

class Peca{
  static int numPretas, numBrancas;
  tiposPeca tipo;
  cores cor;
public:
    //Construtor
    Peca(tiposPeca tipo, cores ccor): tipo(tipo), cor(cor){
      if(cor == BRANCA)
        numBrancas++;
      if(cor == PRETA)
        numPretas++;
    }
    //Construtor Default
    Peca(): tipo(VAZIO), cor(NEUTRO) {
    }
    // Destrutor
    ~Peca() {
      if(cor == BRANCA)
        numBrancas--;
      if(cor== PRETA)
        numPretas--;
    }
    cores getCor(){
      return cor;
    }
    tiposPeca getTipo() {return tipo;}
    void setCor(cores val){
      cor = val;
    }
};

int Peca::numPretas = 0;
int Peca::numBrancas = 0;

class Rainha: private Peca{
};
class Comum: public Peca{
public:
  Comum(cores cor): Peca(COMUM, cor) {}
};


class Tabuleiro{
  int proxJogador;
  Peca *tabuleiro[8][8];
public:
  Tabuleiro(): tabuleiro{NULL}, proxJogador(0) {
    for(int i =0; i<8; i++){
      for(int j= 0; j<8; j++) {
        if((i == 0 || i ==2)  && (j%2)) {
          Comum *pA = new Comum(PRETA);
          tabuleiro[i][j] = pA;
        }
        if(i == 1 && !(j%2)){
          Comum *pB = new Comum(PRETA); 
          tabuleiro[i][j] = pB;
        }
        if((i == 5 || i ==7)  && !(j%2)) {
          Comum *pC = new Comum(BRANCA);
          tabuleiro[i][j] = pC;
        }
        if(i == 6 && (j%2)) {
          Comum *pD = new Comum(BRANCA);
          tabuleiro[i][j] = pD;
        }
      }
    }
  }
  ~Tabuleiro(){
  }
  friend class Peca;
  void imprimirTabuleiro(){
    for(int i = 0; i<8; i++){
      cout << 8-i << "  ";
      for(int j = 0; j<8; j++) {
        if(tabuleiro[i][j] == NULL)
          cout << "  ";
        else {
          cores cor = tabuleiro[i][j]->getCor();
          cout << " " << letraCores[cor];
        }
        if(j+1 < 8)
          cout << " |";
      }
      if(i+1<8)
        cout <<endl << "   -------------------------------" << endl;
      else
        cout << '\n' <<endl;
    }
    cout << "    ";
    for(int k=0; k<8; k++)
      cout << letrasColunas[k] << "   ";
    cout << endl;
  }
  Tabuleiro(const Tabuleiro &s){
  } //construtor por cópia
  void jogar(char xi, int yi, char xf, int yf);

  bool verificarJogada(int xi, int yi, int xf, int yf) {
    // 1. Verificar limites do tabuleiro
    // 2. VerificarDestino()

    if(xi >= 8 || yi >= 8 || xf >= 8 || yf >= 8 || xi < 0 || yi < 0 || xf < 0 || yf < 0)
      cout << "Jogada Invalida!!!" << endl;
    else
      // tabuleiro[xi][yi]->verificarDestino(xi, yi, xf, yf);
    
    return true;
  }
};

int main(){

  Tabuleiro tab;
  tab.imprimirTabuleiro();

  return 0;
}