const sharedMemoryAddon = require("./build/Release/sharedMemory");

function openSharedMemory(name, sharedMemoryAccess, memorySize) {
  const handle = Buffer.alloc(sharedMemoryAddon.sizeof_SharedMemoryHandle);

  const res = sharedMemoryAddon.OpenSharedMemory(
    name,
    sharedMemoryAccess,
    memorySize,
    handle
  );

  if (res > 0) {
    throw `could not open file mapping for object ${name}: ${res}`;
  } else if (res < 0) {
    throw `could not map view of file ${name}: ${res}`;
  }

  return handle;
}

module.exports = {
  openSharedMemory,
  sharedMemoryAccess: {
    Read: 0x0004,
    Write: 0x0002,
    ReadWrite: 0x0004 | 0x0002,
  },
};
