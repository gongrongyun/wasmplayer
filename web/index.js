
const worker = new Worker('./worker.js');
console.log(worker)
worker.postMessage({ type: 'init' });