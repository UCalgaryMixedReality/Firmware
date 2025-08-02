import serial
import threading
import time
import dearpygui.dearpygui as dpg

dpg.create_context()

# Global buffer for the graph
x_data = []
y1_data = []
y2_data = []
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

                parts = line.split()

                if len(parts) != 2:
                    print(f"Invalid line: {line}")
                    continue

                y1 = float(parts[0])
                y2 = float(parts[1])

                y1_data.append(y1)
                y2_data.append(y2)
                if len(x_data) > max_points:
                    x_data.pop(0)
                    y1_data.pop(0)
                    y2_data.pop(0)
                dpg.configure_item("line1", x=x_data, y=y1_data)
                dpg.configure_item("line2", x=x_data, y=y2_data)

                dpg.fit_axis_data("xaxis")
                dpg.fit_axis_data("yaxis")
            except ValueError:
                print("Invalid data:", line)
    except serial.SerialException as e:
        print("Serial error:", e)

# Dear PyGui setup
def setup_gui():
    with dpg.window():
        with dpg.plot(label="ESP Sensor Data", width=600, height=400):
            dpg.add_plot_legend()
            dpg.add_plot_axis(dpg.mvXAxis, label="Time", tag="xaxis")
            with dpg.plot_axis(dpg.mvYAxis, label="Temperature", tag="yaxis"):
                dpg.add_line_series([], [], tag="line1", parent="yaxis")
                dpg.add_line_series([], [], tag="line2", parent="yaxis")
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


