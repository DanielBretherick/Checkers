//Jogo de Damas


#include <iostream>
#include <math.h>
#include <vector>


using namespace std;

enum cores {BRANCA, PRETA, NEUTRO};
enum tiposPeca {COMUM, RAINHA, VAZIO};
enum colunas {A, B, C, D, E, F, G, H};

vector<char> letrasColunas({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}) ;
char letraCores[2] = {'B', 'P'};

class Peca{
protected:
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
    virtual bool verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {};
};

int Peca::numPretas = 0;
int Peca::numBrancas = 0;

class Rainha: public Peca {
public:
  Rainha(cores cor): Peca(RAINHA, cor) {}
  bool verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {
    // 1. Movimento é diagonal
    // 2. Verificar se há peça no destino final
    // 3. Se for so 1 casa dar ok
    // 4. Se for mais de 1 casa, verificar se há peças inimigas no caminho
    int dl = lf -li;
    int dc = cf -ci;
    if(abs(dl)!=abs(dc)) {return false;}
    if(tb.tabuleiro[lf][cf] != NULL) {return false;}
    if(abs(dl) == 1 && abs(dc) == 1) {return true;}
    if(abs(dl) > 1 && abs(dc) > 1) {
      int sinalC = dc/abs(dc);
      int sinalL = dl/abs(dl);
      for(int l=lf-2*sinalL,c=cf-2*sinalC ; l<lf ; ) {
        if(tb.tabuleiro[l][c] != NULL) {
          return false;
        }
        l+=sinalL;
        c+=sinalC;
      }
      if(tb.tabuleiro[lf-sinalL][cf-sinalC] != NULL) {
        if(tb.tabuleiro[lf-sinalL][cf-sinalC]->getCor() != cor)
          return true;
      }
    }
    return false;
  }
};
class Comum: public Peca{
public:
  Comum(cores cor): Peca(COMUM, cor) {}
  bool verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {
    // 1. Movimento é diagonal
    // 2. Se for 1 casas, verificar se há peças no destino
    // 3. Se for 2 casas, verificar se há peça inimiga no caminho e se há peças no destino
    int dl = lf -li;
    int dc = cf -ci;
    if(dl<0) {return false;}
    if(tb.tabuleiro[lf][cf] != NULL) {return false;}
    if(dl == 1 && abs(dc) == 1) {return true;}
    if(dl == 2 && abs(dc) == 2) {
      int sinal = dc/abs(dc);
      if(tb.tabuleiro[li+1][ci+sinal] != NULL) {
        if(tb.tabuleiro[li+1][ci+sinal]->getCor() != cor)
          return true;
      }
    }
    return false;
  }
};


class Tabuleiro{
  int proxJogador;
  Peca *tabuleiro[8][8];
public:
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
  friend class Comum;
  friend class Rainha;
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
  void jogar(char li, int ci, char lf, int cf);
  bool verificarJogada(int li, int ci, int lf, int cf) {
    if(li >= 8 || ci >= 8 || lf >= 8 || cf >= 8 || li < 0 || ci < 0 || lf < 0 || cf < 0)
      cout << "Jogada Invalida!!!" << endl;
    else
      tabuleiro[li][ci]->verificarDestino(li, ci, lf, cf, *this);
    
    return true;
  }
};

int main(){

  Tabuleiro tab;
  tab.imprimirTabuleiro();


  

  return 0;
}