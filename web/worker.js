const BASE_URL = 'http://127.0.0.1:3000'
const GLUEJSURL = BASE_URL + '/core/glue.js'
const GLUEWASMURL = BASE_URL + '/core/'

const fetchCode = async (path) => {
  const res = await fetch(path)
  return await res.text();
}

const createJavascriptURL = async (path) => {
  const code = await fetchCode(path)
  return URL.createObjectURL(new Blob([code], {
    type: 'application/javascript'
  }))
}

const receive = (data) => {
  console.log(data)
}

class Bridge {
  constructor() {
    globalThis.onmessage = this.handleMessage.bind(this)
  }

  async init(url) {
    try {
      const glueScriptUrl = await createJavascriptURL(GLUEJSURL)
      globalThis.importScripts(glueScriptUrl)
      console.log(globalThis)
      this.instance = await globalThis.WASMPlayer({
        mainScriptUrlBlob: glueScriptUrl,
        locateFile: (path) => {
          console.log(path)
          return `${GLUEWASMURL}/${path}`
        }
      })
      console.log('instance', this.instance)
      const addr = this.instance.addFunction(receive, 'vii');
      this.instance._prepare(this.str2Bf(url), addr)
      this.sendMessage({ type: 'wasm_ready' })
    } catch (e) {
      console.error(e)
    }
  }

  sendMessage(data) {
    globalThis.postMessage(data);
  }

  handleMessage({ data }) {
    const { type, payload } = data
    console.log('message', data)
    switch (type) {
      case 'init':
        console.log('call init')
        this.init(payload);
      default:
        break;
    }
  }

  str2Bf (str) {
    return this.instance.allocate(this.instance.intArrayFromString(str), this.instance.ALLOC_NORMAL)
  }
}

const bridge = new Bridge()