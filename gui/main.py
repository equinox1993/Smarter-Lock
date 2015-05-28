__author__ = 'Edwin'

from PyQt5 import QtWidgets
import sys

from controller import UiController

if __name__ == '__main__':
    app = QtWidgets.QApplication(sys.argv)
    controller = UiController()
    controller.start()
    sys.exit(app.exec_())