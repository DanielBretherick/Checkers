//Jogo de Damas
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>

#define WARNING "*** ---"


using namespace std;

enum cores {BRANCA, PRETA, NEUTRO};
enum tiposPeca {COMUM, RAINHA, VAZIO};
enum colunas {A, B, C, D, E, F, G, H};

vector<char> letrasColunas({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}) ;
char letraCores[4] = {'b', 'p', 'B', 'P'};

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
  static int getNumBrancas() { return numBrancas; }
  static int getNumPretas() { return numPretas; }
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
  Comum(const Comum &c): Peca(c.tipo, c.cor) {}
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
          cout << " " << (tabuleiro[i][j]->getTipo() == RAINHA ? letraCores[cor+2] : letraCores[cor]) ;
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
    cout << endl << endl;
    cout << "Nº de peças BRANCAS: " << Peca::getNumBrancas() << endl;
    cout << "Nº de peças PRETAS: " << Peca::getNumPretas();
    cout << endl << endl;
  }

  void transformarCoordenadas(int &li, char &ci, int &lf, char &cf){
    int i = count(letrasColunas.begin(), letrasColunas.end(), ci);
    int j = count(letrasColunas.begin(), letrasColunas.end(), cf);
    if(i == 0 || j == 0)
      cout << "*** --- Letra invalida --- ***"<< endl;
    else {
      for(int j = 0; j<8; j++){
        if(ci == letrasColunas[j])
          ci = j;
        if(cf == letrasColunas[j])
          cf = j;
      }
      li = 8 - li;
      lf = 8 - lf;
      ci = (int)ci;
      cf = (int)cf;
    }
  }

  void solicitarJogada() {
    int li, lf;
    char cci, ccf;
    cout << "\n********** TURNO DO JOGADOR " << letraCores[proxJogador+2] << " **********\n" << endl;
    cout << "Escolha a posição da peça a ser movida: ";
    cin >> li >> cci;

    if(cci >= 'a' && cci <= 'z') {
      cci+= ('A'-'a');
    }

    cout << "Escolha o destino do movimento: ";
    cin >> lf >> ccf;
    cout << endl;
    
    if(ccf >= 'a' && ccf <= 'z') {
      ccf+= ('A'-'a');
    }

    jogar(li, cci, lf, ccf);
  }

  void jogar(int li, char cci, int lf, char ccf) {
    transformarCoordenadas(li, cci, lf, ccf);
    int ci = (int)cci;
    int cf = (int)ccf;
    if(!verificarJogada(li,ci,lf,cf)) {
      cout << "!!!!!!!!! Insira outra jogada !!!!!!!!!\n" << endl;
      solicitarJogada();
    }
    else{
      tabuleiro[lf][cf] = tabuleiro[li][ci];
      tabuleiro[li][ci] = NULL;

      int sinalL = (lf-li)/abs((lf-li));
      int sinalC = (cf-ci)/abs((cf-ci));

      if(tabuleiro[lf-sinalL][cf-sinalC] != NULL) {
        delete tabuleiro[lf-sinalL][cf-sinalC];
        tabuleiro[lf-sinalL][cf-sinalC] = NULL;
      }
      imprimirTabuleiro();
      proxJogador = (proxJogador == BRANCA) ? PRETA : BRANCA;
      solicitarJogada();
    }
   }

  bool verificarJogada(int li, int ci, int lf, int cf) {
    if(li > 8 || ci > 8 || lf > 8 || cf > 8 || li <= 0 || ci < 0 || lf <= 0 || cf < 0) {
      cout << "*** --- Posicao fora do Tabuleiro --- ***" << endl;
      return false;
    }
    else {
      if(tabuleiro[li][ci] == NULL){
        cout << "*** --- Posição sem peças --- ***" << endl;
        return false;
        }
      else
        if(tabuleiro[li][ci]->getCor() != proxJogador){
          cout << "*** --- Peça de outro jogador --- ***" << endl;
          return false;
          }
    }
    return tabuleiro[li][ci]->verificarDestino(li, ci, lf, cf, *this);
  }
};

bool Comum::verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {
  int dl = li -lf;
  int dc = cf -ci;
  if(abs(dl) != abs(dc)) { 
    cout << "*** --- Movimento não foi diagonal --- ***" << endl;
    return false;
  }
  if((dl<0 && (cor == BRANCA))  || (dl > 0 && (cor == PRETA)) ) {
    cout << "*** --- Indo para tras --- ***" << endl; 
    return false;
  }
  if(tb.tabuleiro[lf][cf] != NULL) {
    cout << "*** --- Posicao ocupada --- ***" << endl;
    return false;
  }
  if(abs(dl) == 1 && abs(dc) == 1) {return true;}
  if(abs(dl) == 2 && abs(dc) == 2) {
    int sinalC = dc/abs(dc);
    int sinalL = -dl/abs(dl);
    if(tb.tabuleiro[li+sinalL][ci+sinalC] == NULL) {
      cout << "*** --- Nao pode pular duas, espaco vazio --- ***" << endl;
      return false;
    }
    else{
      Peca *aux = tb.tabuleiro[li+sinalL][ci+sinalC];
      if(aux->getCor() == cor){
        cout << "*** --- Nao pode comer a própria peca --- ***" << endl;
        return false;
      }
      return true;
    }

  }
  else{
    cout << "*** --- Mais de duas casas andadas --- ***" << endl;
    return false;
  }
}

bool Rainha::verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {
    int dl = lf -li;
    int dc = cf -ci;
    if(abs(dl)!=abs(dc)) {return false;}
    if(tb.tabuleiro[lf][cf] != NULL) {return false;}
    if(abs(dl) == 1 && abs(dc) == 1) {return true;}
    if(abs(dl) > 1 && abs(dc) > 1) {
      int sinalC = dc/abs(dc);
      int sinalL = -dl/abs(dl);
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
  tab.solicitarJogada();


  return 0;
}
