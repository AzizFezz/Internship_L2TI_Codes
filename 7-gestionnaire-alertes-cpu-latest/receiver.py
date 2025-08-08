from flask import Flask, request
import subprocess

app = Flask(__name__)

@app.route('/alert', methods=['POST'])
def handle_alert():
    data = request.json
    print("🔔 Alerte reçue :", data)

    for alert in data.get("alerts", []):
        alertname = alert["labels"].get("alertname")
        pod = alert["labels"].get("pod", "")
        namespace = alert["labels"].get("namespace", "default")
        cpu_str = alert["annotations"].get("cpu_usage", "0")
        cpu = float(cpu_str)

        print("Alerte: {} | Pod: {} | Namespace: {} | CPU: {}%".format(alertname, pod, namespace, cpu))

        if alertname == "PodHighCpuUsage":
            print("⚠️ Charge CPU Élevée → Exécution du script label_cpu_moderate")
            subprocess.Popen(["./label_cpu_moderate", pod, namespace])

        elif alertname == "PodCpuFailure":
            print("🔥 PANNE CPU → Exécution du script label_cpu_critical")
            subprocess.Popen(["./label_cpu_critical", pod, namespace])

        elif alertname == "PodCpuNormal":
            print("✅ CPU normal → Exécution du script label_cpu_normal")
            subprocess.Popen(["./label_cpu_normal", pod, namespace])

        else:
            print("❓ Alerte inconnue, aucun script exécuté.")

    return '', 200

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)
