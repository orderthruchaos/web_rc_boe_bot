const http = require('http')
const url = require('url')
const querystring = require('querystring')
const buffer = require('buffer')

const hostname = '127.0.0.1'
const port = 3000

// let LightBlueSDK = require('bean-sdk/build/lightblue')
// let sdk = new LightBlueSDK()
let step_size      = require('./constants').step_size
let sdk            = require('bean-sdk/build/lightblue').sdk()
let boe_bot_rcv_device = require('./known_devices').boe_bot_rcv_device

var boe_bot_receiver = null


// This is (mostly) straight out of the Node.js Bean SDK:
function connectToBean(sdk, name, address, successCallback, errorCallback) {

  if (!name && !address) {
    errorCallback("Please provide bean name or address")
  }

  let found = false

  sdk.startScanning(15, ()=> {
    // Scan timeout
    if (!found) {
      errorCallback('No bean found!')
    }
  })

  sdk.on('discover', (device)=> {
    if (device.getName() === name || device.getAddress() === address) {

      boe_bot_receiver = device
      console.log(`\nFound Bean with name/address: ${device.getName()}/${device.getAddress()}`)
      found = true
      sdk.stopScanning()
      sdk.connectToDevice(device.getAddress(), (err)=> {

        if (err) {
          errorCallback(`Bean connection failed: ${err}`)
          return
        }

        device.lookupServices((err)=> {

          if (err) {
            errorCallback(`Service lookup FAILED: ${err}`)
          } else {
            device.setAutoReconnect(true)
            console.log('Connected!')
            successCallback(device)
          }

        })
      })
    }
  })
}


const process_query = function(dev, q) {
  let d   = null;

  // console.log(q)
  if (q.h) {
    d = `h=${q.h}\n`
  } else if (q.f) {
    d = `f=${q.f}\n`
  } else if (q.b) {
    d = `b=${q.b}\n`
  } else if (q.r) {
    d = `r=${q.r}\n`
  } else if (q.l) {
    d = `l=${q.l}\n`
  }

  // Parse data into buffer
  if (d) {
    console.log(d)
    let buf = new buffer.Buffer(d, 'ascii')

    dev.sendSerial(buf, (err)=> {
      if (err)
        console.warn(`Could not send data: ${d} : (${err})`)
        // throw new Error(err)
      // completedCallback(null)
    })
  }
}


const server = http.createServer((req, res) => {
  res.setHeader('Content-Type', 'text/plain')
  console.log(req.url)
  if (req.url == '/favicon.ico') {
    res.statusCode = 501
    res.end('Do not handle /favicon.ico request\n')
  } else if (boe_bot_receiver) {
    res.statusCode = 200
    let qs = url.parse(req.url).query
    let q  = qs
    if (typeof(q) == "string") { q = querystring.parse(qs) }
    console.log(q);
    process_query(boe_bot_receiver, q)
    res.end(`Data will be sent to ${boe_bot_receiver.getName()}\n`)
  } else {
    res.statusCode = 501
    res.end('Receiver not ready\n')
  }
});


function is_receiver_address(fnd_address, rcv_address) {
  console.log("testing: " + fnd_address)
  if (rcv_address instanceof Array) {
    if (rcv_address.indexOf(fnd_address) >= 0) {
      console.log("yes: " + rcv_address)
      return true;
    }
  } else {
    if (fnd_address == rcv_address) {
      console.log("yes: " + rcv_address)
      return true;
    }
  }
  console.log("no: " + rcv_address)
  return false;
}


sdk.on('discover', (scannedDevice)=> {
  // console.log(scannedDevice)

  let fnd_address = scannedDevice.getAddress()
  let rcv_address = boe_bot_rcv_device.address

  console.log("dev addr: " + fnd_address)
  console.log("rcv addr: ");
  console.log(rcv_address)

  // if (fnd_address == boe_bot_rcv_device.address) {}
  if (is_receiver_address(fnd_address, boe_bot_rcv_device.address)) {
    sdk.stopScanning();
    sdk.connectScannedDevice(scannedDevice, (err, device)=> {
      if (err) {
        console.log(`Bean connection failed: ${err}`)
      } else {
        // device.setAutoReconnect(true)
        device.lookupServices((err)=> {
          if (err) {
            console.log(`Service lookup FAILED: ${err}`)
          }  else {
            boe_bot_receiver = device;
            console.log('Connected!\n')
          }
        })
      }
    })
  }

  console.log(`Receiver name: ${scannedDevice.getName()}`)
})


// Set up LBB
sdk.startScanning()


// Start web server
server.listen(port, hostname, () => {
  console.log(`Server running at http://${hostname}:${port}/`);
})


