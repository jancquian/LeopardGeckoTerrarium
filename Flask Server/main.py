from flask import Flask, render_template, request, jsonify, redirect, url_for

app = Flask(__name__, template_folder='C:/Users/jancq/Escritorio/Flask/app/templates')

dh_eleven = {
    "dato_1": 0.0,
    "dato_2": 0.0,
    "dato_3": 0.0,
    "dato_4": 0.0,
    "dato_5": 0.0,
    "dato_6": 0.0,
    "dato_7": 0.0,
    "dato_8": 0.0
}

vpwm_value = 0
temp_min = 0.0
temp_max = 20.0

@app.route('/')
def index():
    return render_template("index.html")


@app.route('/config', methods=['GET'])
def datos_formulario():
    return render_template("config.html")


@app.route('/obtener_configuracion', methods=['POST'])
def obtener_configuracion():
    global vpwm_value
    global temp_min
    global temp_max
    if request.method == 'POST':
        vpwm_value = request.form['lampwm']
        temp_min = request.form['temin']
        temp_max = request.form['temax']
        print("valor {0}".format(vpwm_value))
    return redirect(url_for('index'))


@app.route('/monitor', methods=['GET'])
def datos_terrario():
    return render_template("monitor.html",  **dh_eleven)


@app.route('/enviar_datos_dh', methods=['POST'])
def recibir_datos():

    global dh_eleven

    try:
        datos_recibidos = request.json

        dh_eleven = {
            "dato_1": datos_recibidos.get("dato_1"),
            "dato_2": datos_recibidos.get("dato_2"),
            "dato_3": datos_recibidos.get("dato_3"),
            "dato_4": datos_recibidos.get("dato_4"),
            "dato_5": datos_recibidos.get("dato_5"),
            "dato_6": datos_recibidos.get("dato_6"),
            "dato_7": datos_recibidos.get("dato_7"),
            "dato_8": datos_recibidos.get("dato_8"),
        }

        # Realizar acciones con los datos recibidos
        print("Datos recibidos:", datos_recibidos)
        # Puedes hacer más cosas aquí con los datos, almacenar en una base de datos, etc.
        return jsonify({"mensaje": "Datos recibidos correctamente"})
    except Exception as e:
        print("Error al procesar datos:", e)
        return jsonify({"error": "Error al procesar datos"})

# AQUI LO RELATIVO AL ESP32

@app.route('/esp32_get_pwm', methods=['GET'])
def esp32_get_pwm():
    global vpwm_value
    return str(vpwm_value)

@app.route('/esp32_get_max', methods=['GET'])
def esp32_get_max():
    global temp_max
    return str(temp_max)

@app.route('/esp32_get_min', methods=['GET'])
def esp32_get_min():
    global temp_min
    return str(temp_min)




if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
