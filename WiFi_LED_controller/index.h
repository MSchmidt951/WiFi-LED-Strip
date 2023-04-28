const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
  <head>
    <style>
      header {
        font-size: 5em;
        font-weight: bolder;
        text-align: center;
        font-variant: small-caps;
        font-family: Impact;
        margin: 25px;
      }
      h1 {
          font-size: 3em;
          font-weight: bolder;
          text-align: center;
          font-variant: small-caps;
          font-family: Impact;
          margin: 10px;
      }
      form {
        display: flex;
        justify-content: center;
        font-size: 2em;
      }
      form * {
        font-size: .8em;
      }
      input {
        margin: 4px;
      }
      body {
        background-color: #060606;
        color: #f4f4fb;
      }
    </style>
    <title>LED strip</title>
  </head>
  <body>
    <header>LED Strip Settings</header>
    <h1>LED strip A</h1>
    <form action="solid">
      Solid colour: <input type="color" name="col" value="ffffff" />
      <input type="submit" value="Set colour" />
      <input type="text" name="LEDstrip" value="0" hidden />
    </form>
    <br /> <form action="rainbowsolid"> <input type="submit" value="Rainbow Solid" /> <input type="text" name="delay" value="50" /> <input type="text" name="LEDstrip" value="0" hidden /> </form>
    <br /> <form action="rainbow">      <input type="submit" value="Rainbow" />       <input type="text" name="delay" value="48" />  <input type="text" name="LEDstrip" value="0" hidden /> </form>
    <hr />
    <h1>LED strip B</h1>
    <form action="solid">
      Solid colour: <input type="color" name="col" value="ffffff" />
      <input type="submit" value="Set colour" />
      <input type="text" name="LEDstrip" value="1" hidden />
    </form>
    <br /> <form action="rainbowsolid"> <input type="submit" value="Rainbow Solid" /> <input type="text" name="delay" value="50" /> <input type="text" name="LEDstrip" value="1" hidden /> </form>
    <br /> <form action="rainbow">      <input type="submit" value="Rainbow" />       <input type="text" name="delay" value="48" />  <input type="text" name="LEDstrip" value="1" hidden /> </form>
  </body>
</html>
)rawliteral";
