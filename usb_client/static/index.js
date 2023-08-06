const checkConnected = function() {
    fetch('/connected').then(response => {
        if (response.ok) 
            response.json().then(x => app.connected = x)
    });
}

const readAllPwm = function() {
    fetch('/pwms')
        .then(response => {
            if (response.ok) 
                response.json().then(x => app.pwms = x)
            });
    
}

const writeAllPwm = function() {
    fetch('/pwms', {method: 'POST', body: JSON.stringify(app.pwms)})
}

const readPwm = function(index) {
    fetch(`/pwm?index=${index}`)
        .then(response => {
            if (response.ok) 
                response.json().then(x => {
                    app.$set(app.pwms, index, x)
                })
            });
}

const writePwm = function(index) {
    fetch('/pwm', {method: 'POST', body: JSON.stringify({index: index, value: app.pwms[index]})})
}

let app = new Vue({
    el:"#app",
    data:{
        connected: false,
        pwms:[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],
    },
    methods: {
        readAllPwm: readAllPwm,
        writeAllPwm: writeAllPwm,
        readPwm: readPwm,
        writePwm: writePwm,
    },
})

setInterval(checkConnected, 500);