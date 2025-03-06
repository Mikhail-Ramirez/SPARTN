from hello import Ui_Form as main
from PyQt5.QtWidgets import QDialog as QType_MainWindow


class MainWindow(QType_MainWindow, main):
    def __init__(self, working_folder, *args, **kwargs):
        super().__init__(*args, **kwargs)
        #self.showFullScreen() # TODO test diff configs to get this to work on diff monitors
        """General Controls"""
        self.ui = main()
        self.ui.setupUi(self)
        self.working_folder = working_folder
        self.output_dir = working_folder

        # Set the window title to "Visual Export Tool"
        self.setWindowTitle("Hello")
        

        #C:\Users\mikhail.ramirez\Desktop\DDN>C:\Users\mikhail.ramirez\AppData\Roaming\Python\Python311\Scripts\pyuic5 .\hello.ui -o hello.py
        #C:\Users\mikhail.ramirez\Desktop\DDN>python laucher.py
        #INFO:root:Begin log: 2024-10-25 09:32:48.389331
        #INFO:root:Main Directory:       C:\Users\mikhail.ramirez\Desktop\DDN
