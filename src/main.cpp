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
        for(int i = 0; i < 3; i++){ // percorrer todo o tabuleiros 
            for(int j = 0; j < 3; j++){
                board[i][j] = ' '; // inicializa todas as posições com espaços vazios 
            }
        }
    }

    void display_board() {
        // Exibir o tabuleiro no console
        system ("clear");
        lock_guard<mutex> lock(board_mutex); //mutex para garantir acesso exclusivo ao tabuleiro durante a execussão da jogada. 
        for(int i = 0; i < 3; i++){ // percorrer todo o tabuleiros 
            for(int j = 0; j < 3; j++){
                cout << board[i][j]; 
                if(j < 1){
                    cout << '|';
                }
            }
            cout << endl;
            if(i < 1){
                cout << "_________________" << endl; 
            }
        cout << "Currente Player: " << current_player << endl;
        }
    }

    bool make_move(char player, int row, int col) {
        // Implementar a lógica para realizar uma jogada no tabuleiro
        // Utilizar mutex para controle de acesso
        // Utilizar variável de condição para alternância de turnos
        unique_lock<mutex> lock(board_mutex);
        if(player != current_player || board[row][col] != ' ' || game_over){
            return false; //Não pode realizar a jogada, caso o jogador não seja o jogador atual ou a opção não esteja vazia ou o jogo tenha acabado.
        }
        board[row][col] = player; //A posição determianda recebe o simbolo do jogador.
        if(check_win(player)){ //verifica se o jogador atual é ganhador. Atualiza a situaçãod o jogo.
            game_over = true;
            winner = player;
        }else if(check_draw()){//verifica se há empate.
            game_over = true;
            winner = 'D';
        }
        if(current_player == 'X'){ //alternar jogador. 
            current_player = 'O';
        }else {
            current_player = 'X';
        }
        turn_cv.notify_all(); //variavel de condição para acordar a thread o outro jogado.
        return true;
    }   

    bool check_win(char player) {
        // Verificar se o jogador atual venceu o jogo
        for(int i = 0; i <= 2; i++){//verificar vitoria nas linhas e colunas
            if(board[i][0] == player && board[i][1] == player && board[i][2] == player ){ //linhas 
                return true;
            }
            if(board[0][i] == player && board[1][i] == player && board[2][i] == player ){ //colunas
                return true;
            } 
        }          
        if(board[0][0] == player && board[1][1] == player && board[2][2] == player ){ //Diagonal 1            
            return true;
        }    
        if(board[0][2] == player && board[1][1] == player && board[2][0] == player ){ //Diagonal 2            
            return true;
        }       
        return false;
    }

    bool check_draw() {
        // Verificar se houve um empate --------- NECESSÁRIO SEMPRE VERIFICAR SE HOUVE VITORIA ANTES
        for(int i = 0; i < 3; i++){ // percorrer todo o tabuleiro para verificar se ainda há opção para jogar. 
            for(int j = 0; j < 3; j++){
                if(board[i][j] == ' '){
                    return false;
                }
            }
        }
    return true;
    }
    bool is_game_over() {
        // Retornar se o jogo terminou
        lock_guard<mutex> lock(board_mutex);
        return game_over;
    }

    char get_winner() {
        // Retornar o vencedor do jogo ('X', 'O', ou 'D' para empate)
        lock_guard<mutex> lock(board_mutex);
        return winner;
    }
    mutex& get_mutex(){
        lock_guard<mutex> lock(board_mutex);
        return board_mutex;
    }
    condition_variable& get_turn(){
        lock_guard<mutex> lock(board_mutex);
        return turn_cv;
    }
    char get_curret_player(){
        lock_guard<mutex> lock(board_mutex);
        return current_player;
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
        while(!game.is_game_over()){
            unique_lock<mutex> lock(game.get_mutex());
            game.get_turn().wait(lock, [&]() { return game.get_curret_player() == symbol || game.is_game_over(); });
            if(strategy == "sequential"){
                play_sequential();
            }else if (strategy == "random"){
                play_random();
            }
        }
    }

private:
    void play_sequential() {
        // Implementar a estratégia sequencial de jogadas
        for(int i = 0; i < 3; i++){ // percorrer todo o tabuleiros 
            for(int j = 0; j < 3; j++){
                if(game.make_move(symbol, i, j)){//jogada realizada.
                    return;
                };
            }
        }
    }
    int row, col;
    void play_random() {
        // Implementar a estratégia aleatória de jogadas
        while(true){
            row = rand() % 3; //numero aleatorio 0 a 2 para a linha.
            col = rand() % 3; //numero aleatorio 0 a 2 para a coluna.
            if(game.make_move(symbol, row, col)){ //jogada realizada.
                return;
            }
            if(game.is_game_over()){// fim de jogo;
                return;
            }
        }
    }
};

// Função principal
int main() {
    // Inicializar o jogo e os jogadores
    TicTacToe jogo('X', false, ' ');
    Player jogador1(jogo, 'O', "random");
    Player jogador2(jogo, 'X', "sequential");
    // Criar as threads para os jogadores
    thread Tjogador1(&Player::play, &jogador1);
    thread Tjogador2(&Player::play, &jogador2);
    // Aguardar o término das threads
    Tjogador1.join();
    Tjogador2.join();
    // Exibir o resultado final do jogo
    if (jogo.get_winner() == 'D') {//empate
        std::cout << "O jogo terminou em empate!" << std::endl;
    } else { //algum ganhador;
        std::cout << "O jogador " << jogo.get_winner() << " venceu!" << std::endl;
    }
    return 0;
}
