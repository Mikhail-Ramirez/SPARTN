import socket
import threading
from kivy.app import App
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.uix.textinput import TextInput
from kivy.uix.widget import Widget
from kivy.graphics import Color, Line, Ellipse, Rectangle
from kivy.properties import ListProperty, ObjectProperty
from kivy.core.window import Window
from kivy.config import Config
from kivy.clock import Clock

class GraphWidget(Widget):
    tower_positions = ListProperty([(0, 0), (0, 0), (0, 0), (0, 0)])
    received_position = ListProperty([0, 0])

    def __init__(self, **kwargs):
        super(GraphWidget, self).__init__(**kwargs)
        self.bind(tower_positions=self.update_canvas)
        self.bind(received_position=self.update_canvas)
        self.bind(size=self.update_canvas)
        self.bind(pos=self.update_canvas)
        self.update_canvas()  # Initial canvas update

    def update_position(self, x, y):
        self.received_position = [x, y]  # Update position on the main thread

    def update_canvas(self, *args): #hi
        self.canvas.clear()
        with self.canvas:
            # Draw background
            Color(0, 1, 0, 1)  # Green background
            Rectangle(pos=self.pos, size=self.size)

            # Draw coordinate axes
            Color(0, 0, 0, 1)  # Black color for axes
            width, height = self.size
            center_x = self.x + width / 2
            center_y = self.y + height / 2
            Line(points=[self.x, center_y, self.x + width, center_y], width=1)  # X-axis
            Line(points=[center_x, self.y, center_x, self.y + height], width=1)  # Y-axis

            # Draw origin point
            Color(0, 0, 1, 1)  # Blue color for origin
            Ellipse(pos=(center_x - 5, center_y - 5), size=(10, 10))

            # Draw towers
            for pos in self.tower_positions:
                x_data, y_data = pos
                x = center_x + (x_data / 100) * (width / 2)
                y = center_y + (y_data / 100) * (height / 2)
                Color(1, 0, 0, 1)  # Red color for towers
                Ellipse(pos=(x - 5, y - 5), size=(10, 10))

            # Draw received position from Raspberry Pi
            x_data, y_data = self.received_position
            x = center_x + (x_data / 100) * (width / 2)
            y = center_y + (y_data / 100) * (height / 2)
            Color(1, 0.5, 0, 1)  # Orange color for received position
            Ellipse(pos=(x - 5, y - 5), size=(10, 10))


class DroneDetectionApp(App):
    def build(self):
        # Set the window size to simulate the tablet in portrait mode
        Window.size = (600, 1024)

        root = BoxLayout(orientation='vertical')
        self.graph_widget = GraphWidget(size_hint_y=None)
        root.add_widget(self.graph_widget)
        self.graph_widget.bind(width=self.adjust_graph_height)
        self.graph_widget.height = self.graph_widget.width = Window.width

        input_layout = BoxLayout(
            orientation='vertical',
            padding=[10, 0, 10, 0],
            spacing=5
        )

        title_label = Label(
            text='Drone Detection Network',
            font_size='24sp',
            bold=True,
            underline=True,
            size_hint_y=None,
            height=40,
            halign='center',
            valign='middle'
        )
        title_label.bind(size=title_label.setter('text_size'))
        input_layout.add_widget(title_label)
        input_layout.add_widget(Widget(size_hint_y=None, height=10))

        self.tower_inputs = []
        for i in range(1, 5):
            tower_label = Label(
                text=f'Tower {i} Location',
                size_hint_y=None,
                height=30,
                halign='center',
                valign='middle'
            )
            tower_label.bind(size=tower_label.setter('text_size'))
            input_layout.add_widget(tower_label)

            xy_layout = BoxLayout(
                orientation='horizontal',
                spacing=5,
                size_hint_y=None,
                height=50
            )
            x_label = Label(text='X:', size_hint_x=None, width=25)
            x_input = TextInput(size_hint=(0.5, None), height=50, multiline=False, font_size='18sp')
            y_label = Label(text='Y:', size_hint_x=None, width=25)
            y_input = TextInput(size_hint=(0.5, None), height=50, multiline=False, font_size='18sp')

            xy_layout.add_widget(x_label)
            xy_layout.add_widget(x_input)
            xy_layout.add_widget(y_label)
            xy_layout.add_widget(y_input)

            input_layout.add_widget(xy_layout)
            if i < 4:
                input_layout.add_widget(Widget(size_hint_y=None, height=10))

            self.tower_inputs.append((x_input, y_input))

        input_layout.add_widget(Widget(size_hint_y=1))
        root.add_widget(input_layout)

        for x_input, y_input in self.tower_inputs:
            x_input.bind(text=self.on_tower_input)
            y_input.bind(text=self.on_tower_input)

        # Start the TCP thread to listen for incoming data
        threading.Thread(target=self.start_tcp_server, daemon=True).start()
        return root

    def adjust_graph_height(self, instance, value):
        self.graph_widget.height = self.graph_widget.width

    def on_tower_input(self, instance, value):
        positions = []
        for x_input, y_input in self.tower_inputs:
            try:
                x = float(x_input.text)
            except ValueError:
                x = 0
            try:
                y = float(y_input.text)
            except ValueError:
                y = 0
            positions.append((x, y))
        self.graph_widget.tower_positions = positions

    def start_tcp_server(self):
        HOST, PORT = "0.0.0.0", 65432
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((HOST, PORT))
            s.listen()
            print("Waiting for connection on 127.0.0.1:65432...")
            conn, addr = s.accept()
            print(f"Connected by {addr}")
            with conn:
                while True:
                    data = conn.recv(1024)
                    if not data:
                        break
                    try:
                        x, y = map(float, data.decode().strip().split(","))
                        # Schedule the update on the main thread
                        Clock.schedule_once(lambda dt: self.graph_widget.update_position(x, y))
                        print(f"Received: x={x}, y={y}")
                    except ValueError:
                        print("Invalid data received")

if __name__ == '__main__':
    DroneDetectionApp().run()
