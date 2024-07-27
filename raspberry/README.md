# raspberry-lab
Repositorio donde se almacenan los archivos y esquemas correspondientes a proyectos usando raspberry


## Para conectarse a la raspberry
Actualmente la raspberry esta configurada para conectarse al WiFi local, si se quiere usar otra red es necesario cambiar las credenciales de red.
Primero tenemos que encontrar la IP privada asignada al raspberry
```
sudo nmap -sP 192.168.1.0-25
sudo nmap -p 22 192.168.1.1,15,17
```

Una vez conseguido nos conectamos
```
ssh raspberry-pi@192.168.1.17 –p gH&aYT270##j
MAC: E4:5F:01:FE:10:ED
```

## Para copiar archivos entre ambientes
Utilizamos scp con el siguiente comando:
```
cd /mnt/c/Users/jugue/Downloads/github/iot-lab
scp -r ./raspberry/cloud/upload_s3 raspberry-pi@192.168.1.17:/home/raspberry-pi –p gH&aYT270##j
```
