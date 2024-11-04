#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <array>


using namespace std;


// Classe TicTacToe
class TicTacToe {
private:
    std::array<std::array<char, 3>, 3> board; // Tabuleiro do jogo
    std::mutex board_mutex; // Mutex para controle de acesso ao tabuleiro
    std::condition_variable turn_cv; // Variável de condição para alternância de turnos
    char current_player; // Jogador atual ('X' ou 'O')
    bool game_over; // Estado do jogo
    char winner; // Vencedor do jogo

public:
    TicTacToe(char current_player, bool game_over,  char winner ):  current_player('X'), game_over(false), winner(' ')  {
        // Inicializar o tabuleiro e as variáveis do jogo
        for(int i = 0; i <= 2; i++){ // percorrer todo o tabuleiros 
            for(int j = 0, j <= 2, j++){
                board[i][j] = ' '; // inicializa todas as posições com espaços vazios 
            }
        }
    }

    void display_board() {
        // Exibir o tabuleiro no console
        for(int i = 0; i <= 2; i++){ // percorrer todo o tabuleiros 
            for(int j = 0; j <= 2; j++){
                cout << board[i][j]; 
                if(j < 1){
                    cout << '|' << 
                }
            }
            cout << endl;
            if(i < 1){
                cout << "_________________" << endl; 
            }
        }
    }

    bool make_move(char player, int row, int col) {
        // Implementar a lógica para realizar uma jogada no tabuleiro
        // Utilizar mutex para controle de acesso
        // Utilizar variável de condição para alternância de turnos
    }

    bool check_win(char player) {
        // Verificar se o jogador atual venceu o jogo
    }

    bool check_draw() {
        // Verificar se houve um empate
    }

    bool is_game_over() {
        // Retornar se o jogo terminou
    }

    char get_winner() {
        // Retornar o vencedor do jogo ('X', 'O', ou 'D' para empate)
    }
};

// Classe Player
class Player {
private:
    TicTacToe& game; // Referência para o jogo
    char symbol; // Símbolo do jogador ('X' ou 'O')
    std::string strategy; // Estratégia do jogador

public:
    Player(TicTacToe& g, char s, std::string strat) 
        : game(g), symbol(s), strategy(strat) {}

    void play() {
        // Executar jogadas de acordo com a estratégia escolhida
    }

private:
    void play_sequential() {
        // Implementar a estratégia sequencial de jogadas
    }

    void play_random() {
        // Implementar a estratégia aleatória de jogadas
    }
};

// Função principal
int main() {
    // Inicializar o jogo e os jogadores
    // Criar as threads para os jogadores

    // Aguardar o término das threads

    // Exibir o resultado final do jogo

    return 0;
}
