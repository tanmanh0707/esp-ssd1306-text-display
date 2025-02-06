#pragma once

const uint8_t index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
  <style>
  table {
    border-collapse: collapse;
    width: 100vw;
  }

  th, td {
    text-align: left;
    padding: 8px;
  }

  tr:nth-child(even) {
    background-color: #D6EEEE;
  }
  .img-container { position: relative; width: auto; height: 48px; padding-bottom: 10px;}
  .img-container .img-checkbox { position: absolute; top: 30px; left: 30px; }
  .img-container p { position: absolute; top: 15px; left: 60px; margin: 0px;}
  .message { width:100%; margin:5px 0; padding:3px; box-sizing: border-box;}
  </style>
</head>

<body>
  <form action='/setmsg' method='POST' id='msg-form'>
    <textarea class='message' id="message" name="message" rows="5"></textarea>
    <input type="submit" value="Cập nhật" />
  </form>
</body>

<script>
const _msgEl=document.getElementById('message');
const _msgFormEl = document.getElementById('msg-form');
_msgFormEl.addEventListener('submit', setMsg);

async function setMsg(event) {
  event.preventDefault();
  var url = _msgFormEl.action;
  console.log('url: ' + url);
  await fetch(url, {method:_msgFormEl.method, body:_msgEl.value});
}

</script>
</html>
)rawliteral";
