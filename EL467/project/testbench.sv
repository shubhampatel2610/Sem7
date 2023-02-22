module testbench;

// input signals
  reg clk;
  reg [3:0] req_floor;

// output signals


// instantiate the elevator controller
  elevator elevatorcontroller(req_floor,clk);

// test bench code
initial begin
  // set initial values for the input signals
  $dumpfile("abc.vcd");
  $dumpvars(0,testbench);
  clk = 0;
 
  req_floor = 4'b0110;

  // run the test for 100 clock cycles
  #20;

  // set the target floor to 3
  req_floor = 4'b001;

  // run the test for 100 clock cycles
  #10;

  // set the target floor to 1
  req_floor = 4'b0101;

  // run the test for 100 clock cycles
  #10;

  // set the target floor to 0
  req_floor = 4'b0000;

  // run the test for 100 clock cycles
  #100;
  //targetFloor=4'b0100;
  #50;
  
  $finish;
end

// clock generator
always #5 clk = ~clk;

endmodule