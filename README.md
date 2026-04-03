# EXERCISE2
[
    {
        "id": "08b62ca19a783dac",
        "type": "tab",
        "label": "Flow 1T",
        "disabled": false,
        "info": "",
        "env": []
    },
    {
        "id": "55b9045b6c49de85",
        "type": "tab",
        "label": "Flow 2T",
        "disabled": false,
        "info": "",
        "env": []
    },
    {
        "id": "e3217fa7b89cd4ca",
        "type": "http in",
        "z": "08b62ca19a783dac",
        "name": "Receive Telemetry",
        "url": "/api/sensor",
        "method": "get",
        "upload": false,
        "skipBodyParsing": false,
        "swaggerDoc": "",
        "x": 230,
        "y": 60,
        "wires": [
            [
                "ab5f0922d05efc3d",
                "5d588685fbf15675"
            ]
        ]
    },
    {
        "id": "8a9c928c9dc7e86d",
        "type": "http response",
        "z": "08b62ca19a783dac",
        "name": " Response",
        "statusCode": "200",
        "headers": {},
        "x": 780,
        "y": 60,
        "wires": []
    },
    {
        "id": "ab5f0922d05efc3d",
        "type": "debug",
        "z": "08b62ca19a783dac",
        "name": "debug 1",
        "active": true,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "true",
        "targetType": "full",
        "statusVal": "",
        "statusType": "auto",
        "x": 760,
        "y": 200,
        "wires": []
    },
    {
        "id": "5d588685fbf15675",
        "type": "function",
        "z": "08b62ca19a783dac",
        "name": "function 1",
        "func": "msg.payload = {\n    sensor_id: \"ESP32_01\",\n    temperature: 24.5,\n    humidity: 60\n};\nreturn msg;",
        "outputs": 1,
        "timeout": 0,
        "noerr": 0,
        "initialize": "",
        "finalize": "",
        "libs": [],
        "x": 520,
        "y": 60,
        "wires": [
            [
                "8a9c928c9dc7e86d"
            ]
        ]
    },
    {
        "id": "d23e9cd83080999f",
        "type": "http in",
        "z": "55b9045b6c49de85",
        "name": "Receive Telemetry",
        "url": "/api/sensor",
        "method": "post",
        "upload": false,
        "skipBodyParsing": false,
        "swaggerDoc": "",
        "x": 170,
        "y": 80,
        "wires": [
            [
                "43d776bf3128b165",
                "13c92be42db5636c"
            ]
        ]
    },
    {
        "id": "13c92be42db5636c",
        "type": "http response",
        "z": "55b9045b6c49de85",
        "name": " Response",
        "statusCode": "200",
        "headers": {},
        "x": 720,
        "y": 80,
        "wires": []
    },
    {
        "id": "43d776bf3128b165",
        "type": "debug",
        "z": "55b9045b6c49de85",
        "name": "debug 2",
        "active": true,
        "tosidebar": true,
        "console": false,
        "tostatus": false,
        "complete": "true",
        "targetType": "full",
        "statusVal": "",
        "statusType": "auto",
        "x": 700,
        "y": 220,
        "wires": []
    }
]
