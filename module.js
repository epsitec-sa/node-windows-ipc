const sharedMemoryAddon = require("./build/Release/sharedMemory");

function isArrayBuffer(value) {
  return (
    value && value.buffer instanceof Buffer && value.byteLength !== undefined
  );
}

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

function writeSharedData(handle, data) {
  let buf = null;

  if (isArrayBuffer(data)) {
    buf = data;
  } else if (data && typeof data === "string") {
    buf = Buffer.from(data, "utf8");
  } else {
    buf = Buffer.from(data);
  }

  const res = sharedMemoryAddon.WriteSharedData(handle, buf, buf.byteLength);

  if (res === 1) {
    throw `data size (${data.length()}) exceeded maximum shared memory size`;
  }
}

function readSharedData(handle, memorySize) {
  const res = sharedMemoryAddon.ReadSharedData(handle, data);

  if (res === 1) {
    throw `data size (${data.length()}) exceeded maximum shared memory size`;
  }
}

function closeSharedMemory(handle) {
  sharedMemoryAddon.CloseSharedMemory(handle);
}

module.exports = {
  createSharedMemory,
  openSharedMemory,
  writeSharedData,
  readSharedData,
  closeSharedMemory,

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
