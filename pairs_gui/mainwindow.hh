/*Class mainWindow
 * ----------
 * COMP.CS.110 SPRING 2021
 * ----------
 * mainwindow of the game
 *
 * Ohjelman kirjoittaja
 * Nimi: Mikko Tuumanen
 * Opiskelijanumero: 284030
 * Käyttäjätunnus: tuumanen ( Git-repositorion hakemistonimi. )
 * E-Mail: mikko.tuumanen@tuni.fi
 **/

#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH

#include "player.hh"

#include <QMainWindow>
#include <iostream>
#include <map>
#include <QDebug>
#include <QPushButton>
#include <QString>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //constructor
    MainWindow(QWidget *parent = nullptr);

    //destructor
    ~MainWindow();

private slots:
    //void on_pushButton_clicked();

    //finds clicked button
    void handleCardButtonClick();

    //starts game if enough players
    void on_startGamePushButton_clicked();

    //adds player if name is not taken or empty
    void on_addPlayerPushButton_clicked();


private:

    Ui::MainWindow *ui;


    // Space between elements, both horizontally and vertically
    const int MARGIN = 20;

    // Constants for push buttons
    const int NARROW_BUTTON_WIDTH = 50; //= 20;
    const int DEFAULT_BUTTON_HEIGTH =60;//= 30;

    // Size of the text browser
    const int DISPLAY_WIDTH = 200;
    const int DISPLAY_HEIGTH = 30;

    // Location of the graphics view and scene
    const int VIEW_X = 20;
    const int VIEW_Y = 350;

    // gameboardgridlayout's position transformation constants
    const int GRID_X = 50;
    const int GRID_Y = 270;

    const int FRUIT_SIZE =80; //= 50;


    const std::vector<std::string>
            fruits_ = {"cherries", "strawberry", "orange", "pear", "apple",
                      "bananas", "tomato", "grapes", "eggplant", "chili"};

    //images and buttons for every fruit
    std::map <std::string,QPixmap> fruit_images_;
    std::map <std::string, std::vector<QPushButton*>> fruit_buttons_;


    //duration of delay after turning second card open
    const int DURATION = 1000;

    //player-objects
    std::vector<Player*> players_;

    //tells if game has started
    bool is_game_going_ = 0;

    Player* player_in_turn_ = nullptr;

    //turned cards. nullpointer if not turned
    QPushButton* turned_card1 = nullptr;
    QPushButton* turned_card2 = nullptr;

    //Picture in back of card
    QPixmap* back_of_card_;

    //contains all card pushbuttons
    std::vector<QPushButton*> cardPushButtons_;

    //total of rows, colomns and card in the card formation
    int rows_total_;
    int colomns_total_;
    int cards_total_;

    //total of pairs not found
    int pairs_left_;

    //Initialization methods for buttons
    void init_cards();

    //set text about players, points and found cards
    void print_players();

    //returns a fruit of the card
    std::string find_fruit(QPushButton* card);

    //turns card if there is not two cards open
    void turn_card(QPushButton* card);

    //moves turn to next player
    void next_turn();

    //calculates the closest total of colomns and rows
    //that gives quadrangle shaped card formation
    void Calculate_factors(int number);

    //checks if turned cardsmakes a pair
    void check_turned_cards();

    //find winner(s) and tells it in infoTextBrowser
    void find_winner();

};
#endif // MAINWINDOW_HH
