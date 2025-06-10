/* Class: MainWindow / file: mainwindow.cpp
 *
 * Ohjelman kirjoittaja
 * Nimi: Mikko Tuumanen
 * Opiskelijanumero: 284030
 * Käyttäjätunnus: tuumanen ( Git-repositorion hakemistonimi. )
 * E-Mail: mikko.tuumanen@tuni.fi
 */


#include "mainwindow.hh"
#include "ui_mainwindow.h"

#include <QLabel>
#include <QPushButton>
#include <algorithm>
#include <random>
#include <QDebug>
#include <QString>
#include <QPixmap>
#include <QTimer>


#include <QHBoxLayout>
#include <QPoint>
#include <QCursor>
#include <QGraphicsView>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    const std::string PREFIX(":/Resources/");
    const std::string SUFFIX(".png");

    // gets images for fruits
    for(std::string fruit : fruits_)
    {

    std::string filename = PREFIX + fruit + SUFFIX;
    QPixmap image(QString::fromStdString(filename));

    // Scaling the pixmap
    image = image.scaled(FRUIT_SIZE, FRUIT_SIZE);

    fruit_images_.insert({fruit, image});

    }

    // gets back image of the card
    QPixmap* back_image = new QPixmap(QString::fromStdString(PREFIX + "fruitcardback" + SUFFIX));
    back_image->scaled(FRUIT_SIZE, FRUIT_SIZE);
    back_of_card_ = back_image;


}


MainWindow::~MainWindow()
{
    // deletes all players
    for (Player* player : players_)
    {
        delete player;
    }

    //deletes all cardpushbuttons
    for (QPushButton* card : cardPushButtons_)
    {
        delete card;
    }

    //deletes back picture of the card
    delete back_of_card_;

    delete ui;
}




void MainWindow::handleCardButtonClick()
{

    // Storing global cursor position
    QPoint global_click_position = QCursor::pos();

    // Counting local cursor position, i.e. decreasing
    // Main Window's location from the global one
    int local_x = global_click_position.x() - geometry().x()-GRID_X;
    int local_y = global_click_position.y() - geometry().y()-GRID_Y;
    QPoint local_click_position = QPoint(local_x, local_y);

    //ui->infoTextBrowser->setText(QString::fromStdString(std::to_string(ui->gameTableGridLayout->geometry().x())));

    //searching button
    for(unsigned int i = 0; i < cardPushButtons_.size(); ++i)
    {

        if(cardPushButtons_.at(i)->geometry().contains(local_click_position))
        {

            turn_card(cardPushButtons_.at(i));

            return; // For efficiency reasons
                    // (only one button can be clicked at a time)
        }
    }
}



void MainWindow::on_startGamePushButton_clicked()
{
    cards_total_ = ui->cardsSpinBox->value();

    // checks if game has enough players
    if (players_.empty())
    {
        ui->infoTextBrowser->setText("Atleast one player needed!");
        return;
    }

    // starts game
    if (not is_game_going_)
    {
        pairs_left_ = cards_total_/2 ;
        init_cards();
        is_game_going_ = 1;
        player_in_turn_ = players_.at(0);
        print_players();

    }
}



void MainWindow::on_addPlayerPushButton_clicked()
{
    QString name = ui->typeNameLine->text();


    if (name != "")
    {

        //checks if name is already in use
        bool is_name_free = 1;
        for (Player* player : players_)
        {
            if (player->get_name() == name.toStdString())
            {
                is_name_free = 0;
            }

        }


        if (is_name_free)
        {

            Player* new_player = new Player(ui->typeNameLine->text().toStdString());
            players_.push_back(new_player);

            //clears name typing line
            ui->typeNameLine->setText("");

            print_players();
        }

        else
        {
            ui->infoTextBrowser->setText("player name already taken!");
        }
    }
}


void MainWindow::init_cards()
{

    Calculate_factors(cards_total_);

    //makes cards for every fruit
    for (int index = 0 ; index < cards_total_/2 ; index++)
    {
        QPushButton* pushButton1 = new QPushButton(this);
        QPushButton* pushButton2 = new QPushButton(this);

        //pushButton1->setFixedWidth(NARROW_BUTTON_WIDTH);
        //pushButton2->setFixedWidth(NARROW_BUTTON_WIDTH);

        pushButton1->setFixedHeight(DEFAULT_BUTTON_HEIGTH);
        pushButton2->setFixedHeight(DEFAULT_BUTTON_HEIGTH);

        cardPushButtons_.push_back(pushButton1);
        cardPushButtons_.push_back(pushButton2);

        fruit_buttons_.insert({fruits_.at(index), {pushButton1, pushButton2}});

    }

    // shuffles cards to random order
    std::random_shuffle(cardPushButtons_.begin(), cardPushButtons_.end());


    int colomn = 0;
    int row = 0;

    // places card to gridlayout in quadrangle shaped card formation
    for (QPushButton* card : cardPushButtons_)
    {
        ui->gameTableGridLayout->addWidget(card, colomn, row);

        if (row == rows_total_-1)
        {
            colomn++;
            row = 0;
        }
        else
        {
            row++;
        }


        card->setIcon(*back_of_card_);

        connect(card, &QPushButton::clicked,
                this, &MainWindow::handleCardButtonClick);

    }


}


void MainWindow::print_players()
{
    std::string text;

    // sets player info text to playersTextBrowser
    for (Player* player : players_)
    {
        text += player->get_name();

        if(player_in_turn_ == player)
        {
            text += " (in turn)";
        }

        text += " ("+std::to_string(player->get_number_of_pairs())+" point(s))";
        text += "\n";

        // Pairs that player has found
        if (player->get_number_of_pairs() > 0)
        {
            text += "has eaten: ";
            for(std::string fruit : player->get_pairs_())
            {
                text += fruit + ", ";
            }

            text += "\n\n";

        }
        else
        {
            text += "\n";
        }
    }

    ui->playersTextBrowser->setText(QString::fromStdString(text));
}


std::string MainWindow::find_fruit(QPushButton* card)
{
    std::map < std::string, std::vector<QPushButton*> >::iterator it;

    //goes through map as long as founds card and its fruit
    for (it = fruit_buttons_.begin(); it != fruit_buttons_.end(); it++)
    {
        if (it->second.at(0) == card or it->second.at(1) == card)
        {
            return it->first;
        }
    }
    return "";
}


void MainWindow::turn_card(QPushButton* card)
{


    if(card==turned_card1)
    {
        return;
    }

    // if two cards has not already turned open
    if (turned_card2 == nullptr)
    {
        std::string fruit = find_fruit(card);
        card->setIcon(fruit_images_.at(fruit));

        if (turned_card1 == nullptr)
        {
            turned_card1 = card;
        }

        // if only one card has turned before
        else
        {
            turned_card2 = card;

            //calls function after delay
            QTimer::singleShot(DURATION, this, &MainWindow::check_turned_cards);


        }
    }

    return;

}


void MainWindow::next_turn()
{
    for (unsigned int index = 0 ; index < players_.size() ; index++)
    {

        if (player_in_turn_ == players_.at(index))
        {
            // next turn is for the next player in the vector
            if (index < players_.size() - 1)
            {
                player_in_turn_ = players_.at(index+1);
            }

            // if player in turn is the last in the vector,
            // next turn is for the first player in vector
            else
            {
                player_in_turn_ = players_.at(0);
            }
            print_players();

            // updates turn information to infoTextBrowser
            ui->infoTextBrowser->setText(
                        QString::fromStdString(player_in_turn_->get_name()+"'s turn"));
            return;
        }
    }
}




void MainWindow::Calculate_factors(int number)
{

    for(int i = 1; i * i <= number; ++i)
    {
        if(number % i == 0)
        {
            rows_total_ = i;
        }
    }
    colomns_total_ = number / rows_total_;
}



void MainWindow::check_turned_cards()
{
    // if turned cards has same fruit
    if(find_fruit(turned_card1) == find_fruit(turned_card2))
    {

        // setting cards to empty position
        turned_card1->setText("empty");
        turned_card2->setText("empty");
        turned_card1->setDisabled(1);
        turned_card2->setDisabled(1);

        //players gets the pair
        player_in_turn_->add_pair(find_fruit(turned_card1));

        print_players();
        pairs_left_ -=1;

    }
    // if cards don't match
    else
    {
        //flipping card to upsidedown
        turned_card1->setIcon(*back_of_card_);
        turned_card2->setIcon(*back_of_card_);

        next_turn();

    }

    // if game has ended, winner have to get found
    if (pairs_left_ == 0)
    {
        find_winner();
    }
        turned_card1 = nullptr;
        turned_card2 = nullptr;
}



void MainWindow::find_winner()
{
    player_in_turn_ =nullptr;
    std::vector<Player*> winners;

    // finds players with the best score
    unsigned int best_score = 0;
    for (Player* player : players_)
    {
        // if player has the best score so far
        if (player->get_number_of_pairs() > best_score)
        {
            winners = {player};
            best_score = player->get_number_of_pairs();
        }

        // if players has same score than the best score so far
        else if (player->get_number_of_pairs() == best_score)
        {
            winners.push_back(player);
        }

    }


    std::string win_text = "GAME OVER!!!!\n";

    //if solo winner
    if (winners.size()==1)
    {
        win_text += "Winner is "+winners.at(0)->get_name();
    }

    //if multiple winners
    if (winners.size()>1)
    {
        win_text += "Winners are ";
        for (unsigned int index = 0 ; index < winners.size() ; index++)
        {

            // making formation of listing winners
            if (index == 0)
            {
            win_text += winners.at(0)->get_name();
            }
            else if (winners.size()-1 > index)
            {
                win_text += ", "+winners.at(index)->get_name();
            }
            else
            {
                win_text += " and " + winners.at(index)->get_name();
            }

        }

    }

    //tells win score
    win_text += "\n with " + std::to_string(best_score) + " pairs!";

    print_players();

    // setting winner info to infoTextBrowser
    ui->infoTextBrowser->setText(QString::fromStdString(win_text));
}




