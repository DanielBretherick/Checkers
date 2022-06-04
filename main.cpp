//Jogo de Damas


#include <iostream>

using namespace std;

enum pecas{BRANCA, PRETA};

class Peca{
    int cor;
public:
    Peca(){
    }
    Peca(int c): cor(c){
    }
    int getCor(){
      return cor;
    }
};

class Rainha: private Peca{
};
class Vazio: private Peca{
};

class Tabuleiro{
    Peca tabuleiro[8][8];
    int proxJogador;
public:
    Tabuleiro():proxJogador(0){
            for(int i =0; i<8; i++){
                for(int j= 0; j<8; j++){
                    if((i == 0 || i ==2)  && !(j%2)){
                        Peca a(PRETA);
                        tabuleiro[i][j] = a;
                        continue;
                    }
                    if(i == 1 && (j%2)){
                      Peca b(PRETA);
                      tabuleiro[i][j] = b;
                      continue;
                    }
                    if((i == 5 || i ==7)  && (j%2)){
                      Peca c(BRANCA);
                      tabuleiro[i][j] = c;
                      continue;
                    }
                    if(i == 6 && !(j%2)){
                      Peca d(BRANCA);
                      tabuleiro[i][j] = d;
                      continue;
                    }
                    else{
                      Peca v(3);
                      tabuleiro[i][j] = v;
                      continue;
                    }
                }
            }
    }
    ~Tabuleiro(){
    }
    void imprimirTabuleiro(){
      for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
          cout << tabuleiro[i][j].getCor() << "|";
        }
        cout <<endl << "----------------" << endl;
      }
    }
    Tabuleiro(const Tabuleiro &s){ //construtor por cópia
    }
};

int main(){
  Peca a(BRANCA);
  Tabuleiro tab;
  tab.imprimirTabuleiro();
  return 0;
}