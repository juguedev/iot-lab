### Referencias
Instalación NodeRed: https://nodered.org/docs/getting-started/local
Tutorial:     https://www.youtube.com/watch?v=JfxBU9G5_Fo&t=1s

```
Install NVM
curl -o- https://raw.githubusercontent.com/nvm-sh/nvm/v0.40.0/install.sh | bash
nvm install 20
nvm use 20

Install NPM
curl -L https://npmjs.org/install.sh | sudo sh

Install Node Red
sudo npm install -g --unsafe-perm node-red

Install DHT Nodes:
node-red-contrib-dht-sensor in node red using the Palette in user settings.

Install GCP Nodes:
npm install node-red-contrib-google-cloud

Load the example provided here: 
    https://www.donskytech.com/node-red-dht11-dht22-custom-weather-station-dashboard/ 
```

### Start and Shut down node-red server

Crear recursos en la nube de GCP.
Luego crear manualmente el API Key y descargar el JSON.
Posteriormente levantar el servidor de node red y exportar el flow.json correspondiente.
```
export GOOGLE_CLOUD_PROJECT=august-tract-432316-e2
node-red
```
Añadir en la sección de key el API Key generado manualmente en GCP.
Probar funcionalidad

Para apagar el servidor:
```
ps -ef | grep node-red
ps -p 2606
sudo kill 9 2606
```
