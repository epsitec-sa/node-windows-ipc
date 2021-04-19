/*global describe,it*/

var assert = require("assert");
var lib = require("../");

describe("CreateSharedMemory", function () {
  it("should create shared memory", function () {
    const handle = lib.createSharedMemory(
      "Local\\CresusUpdaterServiceStateSharedMemory",
      lib.sharedMemoryPageAccess.ReadWrite,
      lib.sharedMemoryFileMapAccess.AllAccess,
      4096
    );

    assert.ok(handle);
  });
});

describe("OpenSharedMemory", function () {
  it("should create and open shared memory", function () {
    const cHandle = lib.createSharedMemory(
      "Local\\CresusUpdaterServiceStateSharedMemory",
      lib.sharedMemoryPageAccess.ReadWrite,
      lib.sharedMemoryFileMapAccess.AllAccess,
      4096
    );
    const oHandle = lib.openSharedMemory(
      "Local\\CresusUpdaterServiceStateSharedMemory",
      lib.sharedMemoryFileMapAccess.AllAccess,
      4096
    );

    assert.ok(cHandle);
    assert.ok(oHandle);
  });
});
