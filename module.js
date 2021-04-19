const sharedMemoryAddon = require("./build/Release/sharedMemory");

function createSharedMemory(name, pageAccess, fileMapAccess, memorySize) {
  const handle = Buffer.alloc(sharedMemoryAddon.sizeof_SharedMemoryHandle);

  const res = sharedMemoryAddon.CreateSharedMemory(
    name,
    pageAccess,
    fileMapAccess,
    memorySize,
    handle
  );

  if (res > 0) {
    throw `could not create file mapping for object ${name}: ${res}`;
  } else if (res < 0) {
    throw `could not map view of file ${name}: ${0 - res}`;
  }

  return handle;
}

function openSharedMemory(name, fileMapAccess, memorySize) {
  const handle = Buffer.alloc(sharedMemoryAddon.sizeof_SharedMemoryHandle);

  const res = sharedMemoryAddon.OpenSharedMemory(
    name,
    fileMapAccess,
    memorySize,
    handle
  );

  if (res > 0) {
    throw `could not open file mapping for object ${name}: ${res}`;
  } else if (res < 0) {
    throw `could not map view of file ${name}: ${0 - res}`;
  }

  return handle;
}

module.exports = {
  createSharedMemory,
  openSharedMemory,

  sharedMemoryPageAccess: {
    ReadOnly: 0x02,
    WriteCopy: 0x08,
    ReadWrite: 0x04,
  },
  sharedMemoryFileMapAccess: {
    Read: 0x0004,
    Write: 0x0002,
    AllAccess: 0xf001f,
  },
};
