# TIE-02100 Johdatus ohjelmointiin
# Mikko Tuumanen, opiskelijanumero: 284030
# Tehtävän 13.10. ratkaisu:
# kaksin vuorollaan pelattava ristinollapeli.
# Pelaaja joka saa ensimmäisenä viisi merkkiään perätysten vaaka, pysty,
# tai diagonaallisessa suunnassa voittaa pelin


from tkinter import *


class Ristinolla:
    def __init__(self):
        """sets beginning information: width and height of game board,
         mark and colour for each player. Also creates main window with labels
         and buttons for game board.
         :param: None
         :return: None"""
        self.__pääikkuna = Tk()
        self.__pääikkuna.title("Tic-tac-toe")
        self.__turn = 0
        self.__players = ["X", "O"]
        self.__playercolours = ["blue", "red"]
        self.__game_ended = False

        self.__leveys = 10
        self.__korkeus = 10
        self.__voittorivin_pituus = 5

        self.__nappularivit = []
        self.__pelilaudan_rivit = []

        # creates buttons of the game board and
        # list of information about every button
        for y in range(self.__korkeus):
            nappularivi = []
            laudan_rivi = []
            for x in range(self.__leveys):
                new_button = Button(self.__pääikkuna, padx=10, pady=5,
                                    text="   ", command=lambda rivi=y,
                                    sarake=x: self.place_mark(sarake, rivi))
                new_button.grid(row=y, column=x+4, sticky=W+E)
                laudan_rivi.append(".")
                nappularivi.append(new_button)
            self.__nappularivit.append(nappularivi)
            self.__pelilaudan_rivit.append(laudan_rivi)

        # creates Start New Game-, and quit -buttons and info label
        self.__newgamebutton = Button(self.__pääikkuna,
                                      command=self.start_new_game,
                                      text="Start New Game")
        self.__newgamebutton.grid(row=3, column=1, columnspan=3)
        self.__infolabel = Label(self.__pääikkuna,
                                 text=self.get_mark()+"'s turn")
        self.__infolabel.grid(row=1, column=1, columnspan=3)
        self.__quitbutton = Button(self.__pääikkuna, command=self.quit,
                                   text="Quit")
        self.__quitbutton.grid(row=6, column=1, columnspan=3)

    def start(self):
        """starts game window
        :param: None
        :return: None"""
        self.__pääikkuna.mainloop()

    def place_mark(self, x, y):
        """Places player's (in turn) mark to selected coordinates
         if possiple due to the rules and changes turn for opposite player
         :param x: int, x-coordinate
                y: int, y-coordinate
         :return: None"""
        if not self.__game_ended and self.__pelilaudan_rivit[y][x] == ".":
            mark = self.get_mark()
            self.__pelilaudan_rivit[y][x] = mark
            self.__nappularivit[y][x].configure(text=mark,
                                        foreground=self.get_player_colour())
            self.check_win()
            self.__turn += 1
            self.update_info()
        else: pass

    def get_player_colour(self):
        """returns player-in-turn's mark colour
        :param: None
        :return colour: str, which tells player's colour"""
        colour = self.__playercolours[self.__turn % len(self.__players)]
        return colour

    def get_mark(self):
        """returns player-in-turn's mark
        :param: None
        :return mark: str, players mark for the play"""
        mark = self.__players[self.__turn % len(self.__players)]
        return mark

    def start_new_game(self):
        """starts new game: clears marks in the game board and
         in the list of game marks. puts turn counter to zero
         and marks the game to be going
         :param: None
         :return: None"""
        self.__turn = 0
        self.__game_ended = False
        self.__infolabel.configure(text=self.get_mark()+"'s turn")

        # clears game board
        for rivi in self.__nappularivit:
            for button in rivi:
                button.configure(text="   ")

        # clears the list of game board information
        for rivi in range(self.__korkeus):
            for sarake in range(self.__leveys):
                self.__pelilaudan_rivit[rivi][sarake] = "."

    def check_win(self):
        """checks if player has won the game
        :param: None
        :return: None"""

        # checks horisontal lines
        for rivi in self.__pelilaudan_rivit:
            line = ""
            for mark in rivi:
                line += mark
            self.check_win_in_line(line)

        # checks vertical lines
        for x in range(self.__leveys):
            line = ""
            for y in range(self.__korkeus):
                line += self.__pelilaudan_rivit[y][x]
            self.check_win_in_line(line)

        # checks diagonal lines where x- and y-coordinate values increases
        # starting from x-axis
        for x_alku in range(self.__leveys):
            x=x_alku
            line = ""
            for y in range(self.__korkeus):
                try:
                    line += self.__pelilaudan_rivit[y][x]
                    x += 1
                except IndexError:
                    break
            self.check_win_in_line(line)

        # checks diagonal lines where x- and y-coordinate values increases
        # starting from y-axis
        for y_alku in range(self.__korkeus):
            y=y_alku
            line = ""
            for x in range(self.__leveys):
                try:
                    line += self.__pelilaudan_rivit[y][x]
                    y += 1
                except IndexError:
                    break
            self.check_win_in_line(line)

        # checks diagonal lines where x-coordinate value decreases and
        # y-coordinate values increases. starting from top side
        for x_alku in range(self.__leveys):
            x=x_alku
            line = ""
            for y in range(self.__korkeus):
                try:
                    line += self.__pelilaudan_rivit[y][x]
                    x -= 1
                except IndexError:
                    break
            self.check_win_in_line(line)

        # checks diagonal lines where x-coordinate value decreases and
        # y-coordinate values increases. starting from right hand side
        for y_alku in range(self.__korkeus):
            y=y_alku
            line = ""
            x = self.__leveys - 1
            while x >= 0:
                try:
                    line += self.__pelilaudan_rivit[y][x]
                    y += 1
                    x -= 1
                except IndexError:
                    break
            self.check_win_in_line(line)

    def check_win_in_line(self, line):
        """checks if player has win in the line
        :param line: str, line for win-check
        :return: None"""
        for player in self.__players:
            if player * self.__voittorivin_pituus in line:
                self.winner(player)

    def winner(self, player):
        """marks game to be ended and
        updates information of winner to infolabel
        :param player: str, winner's play mark
        :return: None"""
        self.__game_ended = True
        self.__infolabel.configure(text=player+" WON THE GAME!!")

    def quit(self):
        """Closes the game window
        :param: None
        :return: None"""
        self.__pääikkuna.destroy()

    def update_info(self):
        """updates player-in-turn-info to info label
        :param: None
        :return: None"""
        if not self.__game_ended:
            self.__infolabel.configure(text=self.get_mark()+"'s turn")


def main():
    ui = Ristinolla()
    ui.start()


main()
