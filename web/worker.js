const fetchCode = async (path) => {
  const res = await fetch(path);
  return await res.text();
};

const createJavascriptURL = async (path) => {
  const code = await fetchCode(path);
  return URL.createObjectURL(
    new Blob([code], {
      type: "application/javascript",
    })
  );
};

const receive = (data) => {
  console.log(data);
};

class Bridge {
  constructor() {
    globalThis.onmessage = this.handleMessage.bind(this);
  }

  async init({ url, glueUrl, wasmUrl }) {
    try {
      const glueScriptUrl = await createJavascriptURL(glueUrl);
      globalThis.importScripts(glueScriptUrl);
      this.instance = await globalThis.WASMPlayer({
        mainScriptUrlBlob: glueScriptUrl,
        locateFile: (path) => {
          return `${wasmUrl}/${path}`;
        },
      });
      console.log(this.instance._prepare);
      const addr = this.instance.addFunction(receive, "vii");
      console.log("ready to prepare");
      console.log(this.instance._prepare(this.str2Bf(url), addr));
      console.log("prepare");
      this.sendMessage({ type: "wasm_ready" });
      console.log("read");
    } catch (e) {
      console.error(e);
    }
  }

  sendMessage(data) {
    globalThis.postMessage(data);
  }

  handleMessage({ data }) {
    const { type, payload } = data;
    switch (type) {
      case "init":
        this.init(payload);
        break;
      case "play":
        this.play();
        break;
      case "pause":
        this.pause();
        break;
      default:
        break;
    }
  }

  play() {
    console.log("worker play");
    this.instance._play();
  }

  pause() {
    this.instance._stop();
  }

  str2Bf(str) {
    return this.instance.allocate(
      this.instance.intArrayFromString(str),
      this.instance.ALLOC_NORMAL
    );
  }
}

const bridge = new Bridge();
