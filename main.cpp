//Jogo de Damas


#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>


using namespace std;

enum cores {BRANCA, PRETA, NEUTRO};
enum tiposPeca {COMUM, RAINHA, VAZIO};
enum colunas {A, B, C, D, E, F, G, H};

vector<char> letrasColunas({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}) ;
char letraCores[2] = {'B', 'P'};

class Peca;
class Comum;
class Rainha;

class Tabuleiro;

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
  virtual bool verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {return false;};
};

int Peca::numPretas = 0;
int Peca::numBrancas = 0;

class Rainha: public Peca {
public:
  Rainha(cores cor): Peca(RAINHA, cor) {}
  bool verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb);
};
class Comum: public Peca{
public:
  Comum(cores cor): Peca(COMUM, cor) {}
  bool verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb);
};

class Tabuleiro{
  cores proxJogador;
  Peca *tabuleiro[8][8];
public:
  Tabuleiro(): tabuleiro{NULL}, proxJogador(BRANCA) {
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

  void transformarCoordenadas(int &li, char &ci, int &lf, char &cf){
   int i = count(letrasColunas.begin(), letrasColunas.end(), ci);
   int j = count(letrasColunas.begin(), letrasColunas.end(), cf);
   if(i == 0 || j == 0)
    cout << "*** --- Letra invalida --- ***"<< endl;
   else{
    for(int j = 0; j<8; j++){
      if(ci == letrasColunas[j])
        ci = j;
      if(cf == letrasColunas[j])
        cf = j;
    }
     li = 8 - li;
     lf = 8 - lf;
  }

  void jogar(int li, char ci, int lf, char cf) {
    transformarCoordenadas(li, ci, lf, cf);

    if(!verificarJogada(lit,cit,lft,cft)) {
      cout << "Jogada inválida" << endl;
    }
      // Equals the destination and the moved piece
      // Equals the inicial space to NULL
      // Delete the taken piece, if it exists, and equal its old space to NULL

      tabuleiro[lft][cft] = tabuleiro[lit][cit];
      tabuleiro[lit][cit] = NULL;

      int sinalL = (lft-lit)/abs((lft-lit));
      int sinalC = (cft-cit)/abs((cft-cit));

      if(tabuleiro[lft-sinalL][cft-sinalC] != NULL) {
        delete tabuleiro[lft-sinalL][cft-sinalC];
        tabuleiro[lft-sinalL][cft-sinalC] = NULL;
      }

   }
  }

  bool verificarJogada(int lit, int cit, int lft, int cft) {
    // 1. Verificar limites do tabuleiro
    // 2. Verificar se há uma peça do seu time no local
    // 3. VerificarDestino()

    if(lit > 8 || cit > 8 || lft > 8 || cft > 8 || lit <= 0 || cit < 0 || lft <= 0 || cft < 0) {
      cout << "*** --- Posicao fora do Tabuleiro --- ***" << endl;
      return false;
    }
    else {
      if(tabuleiro[lit][cit] == NULL)
        return false;
      else
        if(tabuleiro[lit][cit]->getCor() != proxJogador)
          return false;
    }

    return tabuleiro[lit][cit]->verificarDestino(lit, cit, lft, cft, *this);
  }
};

bool Comum::verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {
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

bool Rainha::verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {
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

int main(){

  Tabuleiro tab;
  tab.imprimirTabuleiro();
  cout << endl;
  tab.jogar(1,'A', 2,'C');


  return 0;
}
