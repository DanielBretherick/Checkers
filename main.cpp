//Jogo de Damas


#include <iostream>
#include <vector>

using namespace std;

enum cores {BRANCA, PRETA, NEUTRO};
enum tiposPeca {COMUM, RAINHA, VAZIO};
enum colunas {A, B, C, D, E, F, G, H};

vector<char> letrasColunas({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}) ;
char letraCores[2] = {'B', 'P'};

class Peca{
  static int numPretas, numBrancas;
  tiposPeca tipo;
  cores cor;
public:
    //Construtor
    Peca(tiposPeca tipo, cores cor): tipo(tipo), cor(cor){
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
    // virtual bool verificarDestino() {};
};

int Peca::numPretas = 0;
int Peca::numBrancas = 0;

class Rainha: private Peca{
};
class Comum: public Peca{
public:
  Comum(cores cor): Peca(COMUM, cor) {}
  // bool verificarDestino(int xi, int yi, int xf, int yf) {
    // 1. Movimento é diagonal
    // 2. Se for 1 casas, verificar se há peças no destino
    // 3. Se for 2 casas, verificar se há peça inimiga no caminho e se há peças no destino
    // if()
  // }
};


class Tabuleiro{
  int proxJogador;
public:
  Peca *tabuleiro[8][8];
  Tabuleiro(): tabuleiro{NULL}, proxJogador(0) {
    for(int i =0; i<8; i++){
      for(int j= 0; j<8; j++) {
        if((j == 0 || j ==2)  && (i%2)) {
          Comum *pA = new Comum(PRETA);
          tabuleiro[j][i] = pA;
        }
        if(j == 1 && !(i%2)){
          Comum *pB = new Comum(PRETA); 
          tabuleiro[j][i] = pB;
        }
        if((j == 5 || j ==7)  && !(i%2)) {
          Comum *pC = new Comum(BRANCA);
          tabuleiro[j][i] = pC;
        }
        if(j == 6 && (i%2)) {
          Comum *pD = new Comum(BRANCA);
          tabuleiro[j][i] = pD;
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

  cout << letraCores[tab.tabuleiro[7-2][4]->getCor()]<< endl;

  

  return 0;
}