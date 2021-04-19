/*global describe,it*/

var assert = require("assert");
var lib = require("../");

describe("OpenSharedMemory", function () {
  it("should open shared memory", function () {
    const handle = lib.openSharedMemory(
      "Global\\CresusUpdaterServiceStateSharedMemory",
      lib.sharedMemoryAccess.Read,
      4096
    );

    assert.ok(handle);
  });
});
