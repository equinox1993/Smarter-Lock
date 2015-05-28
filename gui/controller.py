__author__ = 'Edwin'

from PyQt5 import QtGui, QtWidgets

from ui5 import Ui_Form

import sys

DEF_QR_SCAN_LOOPS = 50


def std_println(str):
    sys.stdout.write(str+'\n')
    sys.stdout.flush()


class UiController(object):
    def __init__(self):
        self.form = Ui_Form()
        self.__bind()
        self.__password = ""
        self.__qr_scan_loops = DEF_QR_SCAN_LOOPS

    def start(self):
        QtWidgets.QMainWindow.showFullScreen(self.form)
        #self.form.show()

    # private
    def __bind(self):
        buttons = self.form.findChildren(QtWidgets.QPushButton)

        for but in buttons:
            if '0' <= but.text() <= '9':
                but.clicked.connect(self.make_act_num(but.text()))

        self.form.clearButton.clicked.connect(self.act_clear)
        self.form.unlockButton.clicked.connect(self.act_unlock)
        self.form.scanButton.clicked.connect(self.act_scan)
        self.form.ringButton.clicked.connect(self.act_ring)

    #actions
    def make_act_num(self, num):
        def act_num():
            self.__password += num
            self.form.label.setText(self.form.label.text() + 'X')

        return act_num

    def act_clear(self):
        self.__password = ''
        self.form.label.setText('')

    def act_unlock(self):
        std_println("unlock " + self.__password)
        self.act_clear()

    def act_scan(self):
        std_println("qrdec " + str(self.__qr_scan_loops))

    @staticmethod
    def act_ring():
        std_println("ring")