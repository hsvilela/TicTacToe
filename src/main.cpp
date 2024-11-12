#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <array>

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
    TicTacToe() {
        // Inicializar o tabuleiro e as variáveis do jogo
        for (int i = 0; i<3; i++){
            for(int j = 0; j<3; j++){
                board[i][j] = ' ';
            }
        }
        current_player = 'X'; // coloquei o jogador inicial sendo o X
        game_over = false;    // jogo não começa como ganho, obviamente
        winner = ' ';         // sem vencedores incialmente
    }

    void display_board() {
        // Exibir o tabuleiro no console
        for (int i = 0; i<3; i++){
            for(int j = 0; j<3; j++){
                std::cout << board[i][j] << " ";
            }
            std::cout<< std::endl;
        }
    }

    bool make_move(char player, int row, int col) {
        // Implementar a lógica para realizar uma jogada no tabuleiro
        // Utilizar mutex para controle de acesso
        // Utilizar variável de condição para alternância de turnos
        std::unique_lock<std::mutex> lock(board_mutex);
        turn_cv.wait(lock, [this, player] {return current_player == player || game_over});
        if (game_over) {
            return false;
        }
        if(row>=0 && row<3 && col>=0 && col<3 && board[row][col]== ' '){

            board[row][col] = player;
            std::cout << "Jogador " << player << " fez a jogada na posição (" << i << ", " << j << ")\n";

            is_game_over(); //checa se o jogo acabou

            current_player = (current_player == 'X') ? 'O' : 'X';
            turn_cv.notify_all();  // Notifica o próximo jogador para sua vez
            return true;
        }
        return false;
    }

    bool check_win(char player) {
        // Verificar se o jogador atual venceu o jogo
        for(int i =0; i<3; i++){
            if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) || 
                (board[0][i]==player && board[1][i]==player && board[2][i]==player)){
                    return true;
                }
        }
        if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
            (board[2][0] == player && board[1][1] == player && board[0][2] == player)) {
                return true;
            }
        return false;
    }

    bool check_draw() {
        // Verificar se houve um empate
        for (int i=0; i<3; i++){
            for (int j=0; j<3; j++){
                if(board[i][j] == ' '){
                    return true;
                }
            }
        }
        return false;
    }

    bool is_game_over() {
        // Retornar se o jogo terminou
        if (check_win('X')) {
            game_over = true;
            return game_over;
        } else if (check_win('O')) {
            game_over = true;
            return game_over;
        } else if (check_win()) {
            game_over = true;
            return game_over;
        }
    }

    char get_winner() {
        // Retornar o vencedor do jogo ('X', 'O', ou 'D' para empate)
            if (check_win('X')) {
            winner = 'X';
            return winner;
        } else if (check_win('O')) {
            win = 'O';
            return winner;
        } else if (check_win()) {
            winner = 'D'; // Empate
            return winner;
        }
    }
}
    

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
        while(!is_game_over){
            if (strategy == "sequential") {
                play_sequential();
            }
            if (strategy =="random") {
                play_random();
            }

            }
        }

    }

private:
    void play_sequential() {
        // Implementar a estratégia sequencial de jogadas
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                if (game.make_move(symbol, i, j)) {
                    return;
                }
            }
        }
    }
    

    void play_random() {
        // Implementar a estratégia aleatória de jogadas
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 2);

        while (!game.is_game_over()) {
            int row = distrib(gen);
            int col = distrib(gen);

            if (game.make_move(symbol, row, col)) {
                return;
            }
        }
    }

// Função principal
int main() {
    // Inicializar o jogo e os jogadores
    TicTacToe game;
    Player jogadorX(game, 'X', "sequencial");
    Player jogadorO(game, 'O', "aleatorio");

// Criar as threads para os jogadores
    std::thread t1([&] { jogadorX.play(); });
    std::thread t2([&] { jogadorO.play(); });

// Aguardar o término das threads
    t1.join();
    t2.join();

    game.display_board();
    char winner = game.get_winner();
    if (winner == 'D') {
        std::cout << "O jogo terminou em empate!" << std::endl;
    } else {
        std::cout << "O vencedor é: " << winner << std::endl;
    }
    // Aguardar o término das threads

    // Exibir o resultado final do jogo

    return 0;
}
