module elevator(input [3:0] req_floor,input clk);
  
  reg floor_status[7:0];//binary list to represent floor_status
  reg [2:0] state=2'b00;
  reg[3:0] up_floor=4'b0000;//represents highest requested floor
  reg[3:0] down_floor=4'b0000;//represents lowest requested floor
  reg[3:0] target_floor=4'b0000;//target floor
  reg[3:0] curr_floor=4'b0000;// current floor
  always @ (*) //intial condition for elevator
    begin
      if(down_floor==up_floor && target_floor==up_floor)
        begin
          target_floor=req_floor;
        end
    end
  always @ (req_floor)//updating floor_status list
  begin
    floor_status[req_floor]=1'b1;//1 represents that the floor is in the list
  end
  
  always @ (*) begin //updating up_floor and down_floor on every request
      if(req_floor>up_floor)
        begin
          up_floor=req_floor;
        end
      else if(req_floor<down_floor)
        begin
          down_floor=req_floor;
        end
     end
  always @(posedge clk) begin
     if(up_floor==0 && down_floor==0 && curr_floor==0)
      begin
        state=2'b00;
        target_floor=0;
      end
  case (state)
    //2'b11 ==> moving up
    //2'b01 ==> moving down
    //2'b00 ==> idel
    // idle state: wait for a target floor
    2'b00:
      if(target_floor>curr_floor)//if target_floor is above curr_floor then set state=2'b11
          begin
            state=2'b11;
          end
      else if(target_floor<curr_floor)
          begin
            state=2'b01;
          end
      else
        state=2'b00;

    2'b11:
      if(floor_status[curr_floor]==1)//move up until last floor reached
          begin
            #10
            floor_status[curr_floor]=0;
          end
      else if(curr_floor==target_floor || curr_floor==7)
          begin
            target_floor=down_floor;
            up_floor=0;
            state=2'b01;
            curr_floor--;
          end  
      else
        begin
          state=2'b11;
          curr_floor++;
        end
 
    2'b01://move down until down floor reached
        if(floor_status[curr_floor]==1)
         begin
            #10
            floor_status[curr_floor]=0;
         end
        else if(curr_floor==target_floor || curr_floor==0)
            begin
              target_floor=up_floor;
              down_floor=0;
              state=2'b11;
              curr_floor++;
            end
        else
          begin
            state=2'b01;
            curr_floor--;
          end     
  endcase
end

      
endmodule