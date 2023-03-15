const url = "http://commondatastorage.googleapis.com/gtv-videos-bucket/sample/BigBuckBunny.mp4"

const worker = new Worker('./worker.js');
console.log(worker)
worker.postMessage({ type: 'init', payload: url });