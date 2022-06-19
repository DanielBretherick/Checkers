//Jogo de Damas
#include <iostream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <chrono>
#include <thread>

#define OPEN_RED "\033[1;31m"
#define OPEN_GREEN "\033[1;32m"
#define OPEN_YELLOW "\033[1;33m"

#define CLOSE_COLOR "\033[0m"



using namespace std;

enum cores {VERDE, VERMELHA, NEUTRO};
enum tiposPeca {COMUM, RAINHA, VAZIO};
enum colunas {A, B, C, D, E, F, G, H};

vector<char> letrasColunas({'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'}) ;
char letrasCores[4] = {'g', 'r', 'G', 'R'};

class Peca;
class Comum;
class Rainha;

class Tabuleiro;

class Peca{
protected:
  static int numVermelha, numVerde;
  tiposPeca tipo;
  cores cor;
public:
  //Construtor
  Peca(tiposPeca tipo, cores cor): tipo(tipo), cor(cor){
    if(cor == VERDE)
      numVerde++;
    if(cor == VERMELHA)
      numVermelha++;
  }
  //Construtor Default
  Peca(): tipo(VAZIO), cor(NEUTRO) {
  }
  // Destrutor
  ~Peca() {
    if(cor == VERDE)
      numVerde--;
    if(cor== VERMELHA)
      numVermelha--;
  }
  cores getCor(){
    return cor;
  }
  tiposPeca getTipo() {return tipo;}
  static int getNumVerde() { return numVerde; }
  static int getNumVermelha() { return numVermelha; }
  virtual bool verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {return false;};
  virtual bool verificarSePodeComer(int li, int ci, int &lf, int &cf, Tabuleiro &tab) {return false; }
  virtual bool verificarSePodeMover(int li, int ci, int &lf, int &cf, Tabuleiro &tab) {return false; }
};

int Peca::numVermelha = 0;
int Peca::numVerde = 0;

class Comum: public Peca{
public:
  Comum(cores cor): Peca(COMUM, cor) {}
  Comum(const Comum &c): Peca(c.tipo, c.cor) {}
  bool verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb);
  bool verificarSePodeComer(int li, int ci, int &lf, int &cf, Tabuleiro &tb);
  bool verificarSePodeMover(int li, int ci, int &lf, int &cf, Tabuleiro &tb);
};

class Rainha: public Peca {
public:
  Rainha(cores cor): Peca(RAINHA, cor) {}
  bool verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb);
  bool verificarSePodeComer(int li, int ci, int &lf, int &cf, Tabuleiro &tb);
  bool verificarSePodeMover(int li, int ci, int &lf, int &cf, Tabuleiro &tb);
};

class Tabuleiro{
  cores proxJogador;
  Peca *tabuleiro[8][8];
public:
  Tabuleiro(): tabuleiro{NULL}, proxJogador(VERDE) {
    for(int i =0; i<8; i++){
      for(int j= 0; j<8; j++) {
        if((j == 0 || j ==2)  && (i%2)) {
          Comum *pA = new Comum(VERMELHA);
          tabuleiro[j][i] = pA;
        }
        if(j == 1 && !(i%2)){
          Comum *pB = new Comum(VERMELHA);
          tabuleiro[j][i] = pB;
        }
        if((j == 5 || j ==7)  && !(i%2)) {
          Comum *pC = new Comum(VERDE);
          tabuleiro[j][i] = pC;
        }
        if(j == 6 && (i%2)) {
          Comum *pD = new Comum(VERDE);
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
          cout << (cor == VERDE ? OPEN_GREEN : OPEN_RED);
          cout << " " << (tabuleiro[i][j]->getTipo() == RAINHA ? letrasCores[cor+2] : letrasCores[cor]) ;
          cout << CLOSE_COLOR;
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
  }

  void transformarCoordenadas(int &li, char &ci, int &lf, char &cf){
    int i = count(letrasColunas.begin(), letrasColunas.end(), ci);
    int j = count(letrasColunas.begin(), letrasColunas.end(), cf);
    if(i == 0 || j == 0)
      cout << "\n*** --- Letra invalida --- ***\n"<< endl;
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

  bool verificarVitoria() {
    return (Peca::getNumVerde() == 0 || Peca::getNumVermelha() == 0) ? true : false;
  }

  void transformarRainha(int l, int c) {
    if(l == 0 && tabuleiro[l][c]->getCor() == VERDE ) { 
      Rainha *pG = new Rainha(VERDE);
      delete tabuleiro[l][c];
      tabuleiro[l][c] = pG;
    }
    if(l == 7 && tabuleiro[l][c]->getCor() == VERMELHA ) { 
      Rainha *pR = new Rainha(VERMELHA);
      delete tabuleiro[l][c];
      tabuleiro[l][c] = pR;
    }
  }

  void solicitarJogada() {
    int li, ci, lf, cf;
    char cci, ccf;
    cout << "Escolha a posição da peça a ser movida (Linha, Coluna): ";
    cin >> li >> cci;
    
    if(cin.fail()){
      cout << "\n********** ERRO NA DIGITACAO DAS POSICOES **********\n" << endl;
      cin.clear();
      cin.ignore();
      solicitarJogada();
    }
    else{
    if(cci >= 'a' && cci <= 'z') {
      cci+= ('A'-'a');
    }

    cout << "Escolha o destino do movimento: ";
    cin >> lf >> ccf;
    cout << '\n' << endl;
    
    if(ccf >= 'a' && ccf <= 'z') {
      ccf+= ('A'-'a');
    }

    transformarCoordenadas(li, cci, lf, ccf);

    ci = (int)cci;
    cf = (int)ccf;

    jogar(li, ci, lf, cf);
    }
  }

  void jogar(int li, int ci, int lf, int cf) {
    if(!verificarJogada(li,ci,lf,cf)) {
      cout << "!----!----! Insira outra jogada !----!----!\n" << endl;
      if(proxJogador == VERDE)
          solicitarJogada();
        else
          jogadaComputador();
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
      transformarRainha(lf, cf);
      imprimirTabuleiro();
      proxJogador = (proxJogador == VERDE) ? VERMELHA : VERDE;

      if(verificarVitoria()) {
        cout << "----------- Vitoria do jogador " << (Peca::getNumVerde() == 0 ? letrasCores[3] : letrasCores[2] );
        cout << " -----------" << endl;
      }
      else {
        if(proxJogador == VERDE)
          solicitarJogada();
        else
          jogadaComputador();
      }
    }
   }

  bool verificarJogada(int li, int ci, int lf, int cf) {
    if(li >= 8 || ci >= 8 || lf >= 8 || cf >= 8 || li < 0 || ci < 0 || lf < 0 || cf < 0) {
      if(proxJogador != VERMELHA)
        cout << "\n*** --- Posicao fora do Tabuleiro --- ***\n" << endl;
      return false;
    }
    else {
      if(tabuleiro[li][ci] == NULL){
        if(proxJogador != VERMELHA)
          cout << "\n*** --- Posição sem peças --- ***\n" << endl;
        return false;
        }
      else
        if(tabuleiro[li][ci]->getCor() != proxJogador){
          if(proxJogador != VERMELHA)
            cout << "\n*** --- Peça de outro jogador --- ***\n" << endl;
          return false;
          }
    }
    return tabuleiro[li][ci]->verificarDestino(li, ci, lf, cf, *this);
  }
  
  void jogadaComputador(){
    int lf;
    int cf;
    cout << "\nO computador " << letrasCores[proxJogador+2] << " está jogando...\n" << endl;
    this_thread::sleep_for(chrono::seconds(1));
    for(int li = 0; li< 8; li++) {
      for(int ci =0; ci<8; ci++) {
        if(tabuleiro[li][ci] != NULL && tabuleiro[li][ci]->getCor() == proxJogador) {
          if(tabuleiro[li][ci]->verificarSePodeComer(li, ci, lf, cf, *this)) {
            jogar(li, ci, lf, cf);
            return;
          }
        }
      }
    }

    for(int li = 0; li< 8; li++) {
      for(int ci =0; ci<8; ci++) {
        if(tabuleiro[li][ci] != NULL && tabuleiro[li][ci]->getCor() == proxJogador) {
          if(tabuleiro[li][ci]->verificarSePodeMover(li, ci, lf, cf, *this)) {
            jogar(li, ci, lf, cf);
            return;
          }
        }
      }
    }
  }
};

bool Comum::verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {
  int dl = li -lf;
  int dc = cf -ci;
  if(abs(dl) != abs(dc)) { 
    if(tb.proxJogador != VERMELHA)
      cout << "\n*** --- Movimento não foi diagonal --- ***\n" << endl;
    return false;
  }
  if((dl<0 && (cor == VERDE))  || (dl > 0 && (cor == VERMELHA)) ) {
    if(tb.proxJogador != VERMELHA)
      cout << "\n*** --- Indo para tras --- ***\n" << endl; 
    return false;
  }
  if(tb.tabuleiro[lf][cf] != NULL) {
    if(tb.proxJogador != VERMELHA)
      cout << "\n*** --- Posicao ocupada --- ***\n" << endl;
    return false;
  }
  if(abs(dl) == 1 && abs(dc) == 1) {return true;}
  if(abs(dl) == 2 && abs(dc) == 2) {
    int sinalC = dc/abs(dc);
    int sinalL = -dl/abs(dl);
    if(tb.tabuleiro[li+sinalL][ci+sinalC] == NULL) {
      if(tb.proxJogador != VERMELHA)
        cout << "\n*** --- Nao pode pular duas, espaco vazio --- ***\n" << endl;
      return false;
    }
    else{
      Peca *aux = tb.tabuleiro[li+sinalL][ci+sinalC];
      if(aux->getCor() == cor){
        if(tb.proxJogador != VERMELHA)
          cout << "\n*** --- Nao pode comer a própria peca --- ***\n" << endl;
        return false;
      }
      return true;
    }
  }
  else{
    if(tb.proxJogador != VERMELHA)
      cout << "\n*** --- Mais de duas casas andadas --- ***\n" << endl;
    return false;
  }
}

bool Comum::verificarSePodeComer(int li, int ci, int &lf, int &cf, Tabuleiro &tb) {
  int sinalL = tb.proxJogador == VERDE ? -1 : 1;
  lf = li + 2*sinalL;
  cf = ci + 2;
  if(tb.verificarJogada(li,ci,lf,cf)) 
    return true;
  cf = ci - 2;
  if(tb.verificarJogada(li,ci,lf,cf)) 
    return true;
  return false;
}

bool Rainha::verificarSePodeComer(int li, int ci, int &lf, int &cf, Tabuleiro &tb) {
  int sinalL, sinalC;
  int l = li - 1;
  int c = ci + 1;
  do {
    if(l==-1) {
        l = c - 1;
        c = 0;
      }
    if(c==8) {
      c = l + 1;
      l = 7;
    }
    if(tb.tabuleiro[l][c] != NULL && tb.tabuleiro[l][c]->getCor() != tb.proxJogador) {
      sinalL = (l-li)/abs(l-li);
      sinalC = (c-ci)/abs(c-ci);
      lf = l + sinalL;
      cf = c + sinalC;
      if(tb.verificarJogada(li,ci,lf,cf))
        return true;
    }
    if(li==l)
      break;
    l = l - 1;
    c = c + 1;
  } while(li!=l);
  l = li - 1;
  c = ci - 1;
  do {
    if(l==-1) {
        l = 6 - c;
        c = 7;
      }
    if(c==-1) {
      c = 6 - l;
      l = 7;
    }
    if(tb.tabuleiro[l][c] != NULL && tb.tabuleiro[l][c]->getCor() != tb.proxJogador) {
      sinalL = (l-li)/abs(l-li);
      sinalC = (c-ci)/abs(c-ci);
      lf = l + sinalL;
      cf = c + sinalC;
      if(tb.verificarJogada(li,ci,lf,cf))
        return true;
    }
    if(li==l)
      break;
    l = l - 1;
    c = c - 1;
  } while(li!=l);
  return false;
}

bool Comum::verificarSePodeMover(int li, int ci, int &lf, int &cf, Tabuleiro &tb) {
  int sinalL = tb.proxJogador == VERDE ? -1 : 1;
  lf = li + sinalL;
  cf = ci + 1;
  if(tb.verificarJogada(li, ci, lf, cf))
    return true;
  cf = ci - 1;
  if(tb.verificarJogada(li, ci, lf, cf))
    return true;
  return false;
}


bool Rainha::verificarSePodeMover(int li, int ci, int &lf, int &cf, Tabuleiro &tb) {
  int sinalL, sinalC;
  int l = li - 1;
  int c = ci + 1;
  do {
    if(l==-1) {
        l = c - 1;
        c = 0;
      }
    if(c==8) {
      c = l + 1;
      l = 7;
    }
    if(tb.tabuleiro[l][c] != NULL && tb.tabuleiro[l][c]->getCor() != tb.proxJogador) {
      sinalL = (l-li)/abs(l-li);
      sinalC = (c-ci)/abs(c-ci);
      lf = l - sinalL;
      cf = c - sinalC;
      if(tb.verificarJogada(li,ci,lf,cf))
        return true;
    }
    if(li==l)
      break;
    l = l - 1;
    c = c + 1;
  } while(li!=l);
  l = li - 1;
  c = ci - 1;
  do {
    if(l==-1) {
        l = 6 - c;
        c = 7;
      }
    if(c==-1) {
      c = 6 - l;
      l = 7;
    }
    if(tb.tabuleiro[l][c] != NULL && tb.tabuleiro[l][c]->getCor() != tb.proxJogador) {
      sinalL = (l-li)/abs(l-li);
      sinalC = (c-ci)/abs(c-ci);
      lf = l - sinalL;
      cf = c - sinalC;
      if(tb.verificarJogada(li,ci,lf,cf))
        return true;
    }
    if(li==l)
      break;
    l = l - 1;
    c = c - 1;
  } while(li!=l);
  return false;
}



bool Rainha::verificarDestino(int li, int ci, int lf, int cf, Tabuleiro &tb) {
    int dl = lf -li;
    int dc = cf -ci;
    if(abs(dl)!=abs(dc)) {
      if(tb.proxJogador != VERMELHA)
        cout << "\n*** --- Movimento não foi diagonal --- ***\n"<< endl;
      return false;
    }
    if(tb.tabuleiro[lf][cf] != NULL) {
      if(tb.proxJogador != VERMELHA)
        cout << "\n*** --- Posição ocupada --- ***\n"<< endl;
      return false;
    }
    if(abs(dl) > 1 && abs(dc) > 1) {
      int sinalC = dc/abs(dc);
      int sinalL = dl/abs(dl);
      for(int l=li+sinalL,c=ci+sinalC ; l!=(lf-sinalL) ; ) {
        if(tb.tabuleiro[l][c] != NULL) {
          if(tb.proxJogador != VERMELHA)
            cout << "\n*** --- O caminho não está livre --- ***\n" << endl;
          return false;
        }
        l+=sinalL;
        c+=sinalC;
      }
      if(tb.tabuleiro[lf-sinalL][cf-sinalC] != NULL) {
        if(tb.tabuleiro[lf-sinalL][cf-sinalC]->getCor() == cor) {
          if(tb.proxJogador != VERMELHA)
            cout << "\n*** --- Não pode pular a própria peça --- ***\n";
          return false;
        }
      }
    }
    return true;
  }

int main(){

  Tabuleiro tab;
  cout << OPEN_YELLOW << "  --------------------------------" << CLOSE_COLOR << endl;
  cout << OPEN_YELLOW << "  ----------- CHECKERS -----------" << CLOSE_COLOR << endl;
  cout << OPEN_YELLOW << "  --------------------------------\n" << CLOSE_COLOR << endl;
  tab.imprimirTabuleiro();
  tab.solicitarJogada();

  

  return 0;
}
