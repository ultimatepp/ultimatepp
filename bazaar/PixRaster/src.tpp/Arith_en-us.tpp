topic "Image arithmetic operations";
[ $$0,0#00000000000000000000000000000000:Default]
[H6;0 $$1,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$2,0#37138531426314131252341829483370:codeitem]
[l288;2 $$3,0#27521748481378242620020725143825:desc]
[0 $$4,0#96390100711032703541132217272105:end]
[{_} 
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Arithmetic operations]]}}&]
[s1; &]
[s2;:Pix`:`:Invert`(`): [@(0.0.255) Pix]_[* Invert]()&]
[s3;%% Inverts image.&]
[s4;%% &]
[s1; &]
[s2;:Pix`:`:AddConstantGray`(int`): [@(0.0.255) Pix]_[* AddConstantGray]([@(0.0.255) int]_[*@3 v
al])&]
[s3;%% Adds a constant gray [%-*@3 val] value to image.&]
[s4;%% &]
[s1; &]
[s2;:Pix`:`:MultConstantGray`(int`): [@(0.0.255) Pix]_[* MultConstantGray]([@(0.0.255) int]_
[*@3 val])&]
[s3;%% Multiply all image pixels by a constant [%-*@3 val] value.&]
[s4;%% &]
[s1; &]
[s2;:Pix`:`:AddGray`(Pix`&`): [@(0.0.255) Pix]_[* AddGray]([@(0.0.255) Pix`&]_[*@3 pix2])&]
[s3;%% Adds all grayscale pixels of [%-*@3 pix2] to current pix`'s 
ones.&]
[s4;%% &]
[s1; &]
[s2;:Pix`:`:SubtractGray`(Pix`&`): [@(0.0.255) Pix]_[* SubtractGray]([@(0.0.255) Pix`&]_[*@3 p
ix2])&]
[s3;%% Subtracts [%-*@3 pix2] grayscale pixels from current pix`'s 
ones.&]
[s4;%% &]
[s0; ]