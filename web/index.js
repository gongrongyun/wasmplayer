const url =
  "http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4";
const glueUrl = location.origin + "/core/glue.js";
const wasmUrl = location.origin + "/core";

const playBtn = document.querySelector("#play");
const pauseBtn = document.querySelector("#pause");

class Player {
  constructor() {
    this.worker = new Worker("./worker.js");
    this.worker.onmessage = this.handleMessage.bind(this);
    this.sendMessage({
      type: "init",
      payload: {
        url,
        glueUrl,
        wasmUrl,
      },
    });
  }

  sendMessage(data) {
    this.worker.postMessage(data);
  }

  handleMessage({ data }) {
    console.log(data);
    const { type, payload } = data;
    switch (type) {
      case "wasm_ready":
        console.log("wasm_ready");
        break;
      default:
        break;
    }
  }

  play() {
    this.sendMessage({ type: "play" });
  }

  pause() {
    this.sendMessage({ type: "pause" });
  }

  destroy() {
    this.worker.terminate();
  }
}

const player = new Player();

playBtn.addEventListener("click", () => player.play());
pauseBtn.addEventListener("click", () => player.pause());
