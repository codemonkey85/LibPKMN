import Genders
from pypkmnsim_swig import *
from PyQt4.QtCore import QChar, QString, QVariant
from  PyQt4.QtGui import QComboBox, QHBoxLayout, QImage, QLabel, QPixmap, QSizePolicy, QVBoxLayout, QWidget

class BasePkmnDisplayWidget(QWidget):
    def __init__(self, pkmn, parent=None):
        QWidget.__init__(self, parent)

        self.b_pkmn = pkmn

        mainLayout = QHBoxLayout()
        leftLabel = QLabel()

        rightString = QString("%1").arg(self.b_pkmn.get_species_name())

        spriteImage = QImage()
        spriteImage.load(QString(self.b_pkmn.get_icon_path()))

        leftLabel.setPixmap(QPixmap.fromImage(spriteImage))
        leftLabel.setFixedSize(spriteImage.size())
        rightLabel = QLabel(rightString)

        rightLabel.setSizePolicy(QSizePolicy.Minimum, QSizePolicy.Fixed)

        mainLayout.addWidget(leftLabel)
        mainLayout.addWidget(rightLabel)

        self.setLayout(mainLayout)

class GamesComboBox(QComboBox):
    def __init__(self, parent=None):
        QComboBox.__init__(self, parent)

        games_vec = get_game_group_list()

        for i,game in enumerate(games_vec):
            self.addItem(QString(game), QVariant(i))

class PokemonListComboBox(QComboBox):
    def __init__(self, game, parent=None):
        QComboBox.__init__(self, parent)

        pokemon_vec = get_pokemon_list(game)

        for i,pkmn in enumerate(pokemon_vec):
            self.addItem(QString(pkmn), QVariant(i))

class SpecPkmnDisplayWidget(QWidget):
    def __init__(self, pkmn, parent=None):
        QWidget.__init__(self, parent)

        self.s_pkmn = pkmn

        images_dir = get_images_dir()
        gen = self.s_pkmn.get_generation()

        mainLayout = QVBoxLayout()
        pokemonLayout = QHBoxLayout()

        itemLayout = QHBoxLayout()

        abilityNatureLayout = QHBoxLayout()

        attackOneLayout = QHBoxLayout()
        attackTwoLayout = QHBoxLayout()
        attackThreeLayout = QHBoxLayout()
        attackFourLayout = QHBoxLayout()

        gender = QChar()
        if self.s_pkmn.get_gender() == Genders.MALE: gender = QChar('M')
        elif self.s_pkmn.get_gender() == Genders.FEMALE: gender = QChar('F')
        else: gender = QChar('N')
        
        pokemonQString = QString()
        if gen == 1:
            pokemonQString = QString("%1 (%2)").arg(
                QString(self.s_pkmn.get_species_name()),
                QString(self.s_pkmn.get_nickname())
            )
        else:
            pokemonQString = QString("%1 (%2 - %3)").arg(
                QString(self.s_pkmn.get_species_name()),
                QString(self.s_pkmn.get_nickname()),
                gender
            )
        pokemonLabel = QLabel(pokemonQString)

        itemQString = QString()
        itemLabel = QLabel()
        if gen >= 2:
            itemQString = QString("Item held: None")
            itemLabel = QLabel(itemQString)

        abilityNatureQString = QString()
        abilityNatureLabel = QLabel()
        if gen >= 3:
            abilityNatureQString = QString("Ability: %1, Nature: %2").arg(
                QString(self.s_pkmn.get_ability()),
                QString(self.s_pkmn.get_nature().get_name())
            )

        moves = self.s_pkmn.get_moves()
        attackOneQString = QString("%1").arg(
            QString(moves.get(0).get_name())
        )
        attackTwoQString = QString("%1").arg(
            QString(moves.get(1).get_name())
        )
        attackThreeQString = QString("%1").arg(
            QString(moves.get(2).get_name())
        )
        attackFourQString = QString("%1").arg(
            QString(moves.get(3).get_name())
        )
        attackOneLabel = QLabel(attackOneQString)
        attackTwoLabel = QLabel(attackTwoQString)
        attackThreeLabel = QLabel(attackThreeQString)
        attackFourLabel = QLabel(attackFourQString)

        #Populate layouts
        pokemonLayout.addWidget(pokemonLabel)
        attackOneLayout.addWidget(attackOneLabel)
        attackTwoLayout.addWidget(attackTwoLabel)
        attackThreeLayout.addWidget(attackThreeLabel)
        attackFourLayout.addWidget(attackFourLabel)

        mainLayout.addLayout(pokemonLayout)
        if gen >= 2:
            itemLayout.addWidget(itemLabel)
            mainLayout.addLayout(itemLayout)
        if gen >= 3:
            abilityNatureLayout.addWidget(abilityNatureLabel)
            mainLayout.addLayout(abilityNatureLayout)
        mainLayout.addLayout(attackOneLayout)
        mainLayout.addLayout(attackTwoLayout)
        mainLayout.addLayout(attackThreeLayout)
        mainLayout.addLayout(attackFourLayout)

        self.setLayout(mainLayout)

class TypesComboBox(QComboBox):
    def __init__(self, parent=None):
        QComboBox.__init__(self, parent)

        types_vec = get_type_group_list()

        for i,pkmn_type in enumerate(types_vec):
            self.addItem(QString(pkmn_type), QVariant(i))
