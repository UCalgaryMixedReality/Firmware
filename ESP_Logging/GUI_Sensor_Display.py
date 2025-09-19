import serial
import threading
import time
import dearpygui.dearpygui as dpg

dpg.create_context()

# Global buffer for the graph
x_data = []
y_data = []
y2_data = []
#y3_data = []
#zeros = [0] * len(y3_data)
max_points = 50  # Max number of points to display
start = time.time()

# Serial port configuration (adjust as needed)
SERIAL_PORT = 'COM3'  # e.g. 'COM4' on Windows or '/dev/ttyUSB0' on Linux
BAUD_RATE = 115200

def read_serial_data():
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE)
        while True:
            line = ser.readline().decode('utf-8').strip()
            try:
                timestamp = time.time() - start
                x_data.append(timestamp)

                if not line:
                    continue
                
                y1 = float(line)
                y2 = float(line)

                y_data.append(y1)
                y2_data.append(y2)
                #y3_data.append(y1)

                if len(x_data) > max_points:
                    x_data.pop(0)
                    y_data.pop(0)
                    y2_data.pop(0)
                dpg.configure_item("right_eye", x=x_data, y=y_data)
                dpg.configure_item("left_eye", x=x_data, y=y2_data)
                
                dpg.fit_axis_data("xaxis")
                #dpg.fit_axis_data("yaxis")
                dpg.set_axis_limits("yaxis", 10, 50)

            except ValueError:
                print("Invalid data:", line)
    except serial.SerialException as e:
        print("Serial error:", e)

# Dear PyGui setup
def setup_gui():
    with dpg.window():
        with dpg.child_window(width=600, height=400, autosize_x=True, autosize_y=True, border=True):
            with dpg.plot(label="Lens Temperature Monitor", width=-1, height=-1):
                dpg.add_plot_legend()
                dpg.add_plot_axis(dpg.mvXAxis, label="Time", tag="xaxis")
                with dpg.plot_axis(dpg.mvYAxis, label="Temperature (C)", tag="yaxis"):
                    dpg.add_line_series([], [], tag="right_eye", parent="yaxis")
                    dpg.add_line_series([], [], tag="left_eye", parent="yaxis")
                    dpg.set_axis_limits_auto("xaxis")
                    dpg.set_axis_limits_auto("yaxis")

            


# Start serial thread and GUI
if __name__ == "__main__":
    dpg.create_viewport(title="UCXR GUI", width=900, height=600)
    dpg.setup_dearpygui()
    dpg.show_viewport()

    setup_gui()

    # Start the serial reading in a background thread
    threading.Thread(target=read_serial_data, daemon=True).start()

    # Run the GUI
    dpg.start_dearpygui()
    dpg.destroy_context()


