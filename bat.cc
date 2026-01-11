local Players = game:GetService("Players")
local UIS = game:GetService("UserInputService")
local RS = game:GetService("RunService")

local player = Players.LocalPlayer
local char = player.Character or player.CharacterAdded:Wait()
local hum = char:WaitForChild("Humanoid")
local root = char:WaitForChild("HumanoidRootPart")

local gui = Instance.new("ScreenGui", player.PlayerGui)
gui.Name = "bat.cc PAID"
gui.ResetOnSpawn = false

local frame = Instance.new("Frame", gui)
frame.Size = UDim2.new(0.42,0,0.68,0)
frame.Position = UDim2.new(0.29,0,0.16,0)
frame.BackgroundColor3 = Color3.fromRGB(25,25,25)
frame.Visible = false
frame.BorderSizePixel = 0

Instance.new("UICorner", frame).CornerRadius = UDim.new(0,14)
local stroke = Instance.new("UIStroke", frame)
stroke.Color = Color3.fromRGB(90,90,90)
stroke.Thickness = 1.5

do
    local dragging, start, pos
    frame.InputBegan:Connect(function(i)
        if i.UserInputType == Enum.UserInputType.MouseButton1 then
            dragging = true
            start = i.Position
            pos = frame.Position
            i.Changed:Connect(function()
                if i.UserInputState == Enum.UserInputState.End then dragging = false end
            end)
        end
    end)
    UIS.InputChanged:Connect(function(i)
        if dragging and i.UserInputType == Enum.UserInputType.MouseMovement then
            local d = i.Position - start
            frame.Position = UDim2.new(pos.X.Scale,pos.X.Offset+d.X,pos.Y.Scale,pos.Y.Offset+d.Y)
        end
    end)
end

local title = Instance.new("TextLabel", frame)
title.Size = UDim2.new(1,0,0.09,0)
title.BackgroundTransparency = 1
title.Text = "bat.cc PAID"
title.Font = Enum.Font.GothamBold
title.TextSize = 26
title.TextColor3 = Color3.new(1,1,1)

local close = Instance.new("TextButton", frame)
close.Size = UDim2.new(0.1,0,0.09,0)
close.Position = UDim2.new(0.9,0,0,0)
close.Text = "✕"
close.Font = Enum.Font.GothamBold
close.TextSize = 22
close.BackgroundColor3 = Color3.fromRGB(40,40,40)
close.TextColor3 = Color3.fromRGB(255,80,80)
Instance.new("UICorner", close).CornerRadius = UDim.new(0,8)
close.MouseButton1Click:Connect(function() frame.Visible = false end)

local function button(txt,y)
    local b = Instance.new("TextButton", frame)
    b.Size = UDim2.new(0.85,0,0.075,0)
    b.Position = UDim2.new(0.075,0,y,0)
    b.Text = txt
    b.Font = Enum.Font.GothamBold
    b.TextSize = 17
    b.BackgroundColor3 = Color3.fromRGB(40,40,40)
    b.TextColor3 = Color3.new(1,1,1)
    Instance.new("UICorner", b).CornerRadius = UDim.new(0,10)
    return b
end

local function labeledSlider(text, y, min, max, default, callback)
    local label = Instance.new("TextLabel", frame)
    label.Position = UDim2.new(0.075,0,y-0.045,0)
    label.Size = UDim2.new(0.85,0,0.04,0)
    label.BackgroundTransparency = 1
    label.Text = text .. ": " .. default
    label.Font = Enum.Font.Gotham
    label.TextSize = 14
    label.TextColor3 = Color3.new(1,1,1)

    local bg = Instance.new("Frame", frame)
    bg.Position = UDim2.new(0.075,0,y,0)
    bg.Size = UDim2.new(0.85,0,0.03,0)
    bg.BackgroundColor3 = Color3.fromRGB(40,40,40)
    Instance.new("UICorner", bg).CornerRadius = UDim.new(1,0)

    local fill = Instance.new("Frame", bg)
    fill.Size = UDim2.new((default-min)/(max-min),0,1,0)
    fill.BackgroundColor3 = Color3.fromRGB(100,100,100)
    Instance.new("UICorner", fill).CornerRadius = UDim.new(1,0)

    local knob = Instance.new("Frame", bg)
    knob.Size = UDim2.new(0,14,0,14)
    knob.Position = UDim2.new(fill.Size.X.Scale,-7,0.5,-7)
    knob.BackgroundColor3 = Color3.fromRGB(230,230,230)
    Instance.new("UICorner", knob).CornerRadius = UDim.new(1,0)

    bg.InputBegan:Connect(function(i)
        if i.UserInputType == Enum.UserInputType.MouseButton1 then
            local conn
            conn = RS.RenderStepped:Connect(function()
                local x = (UIS:GetMouseLocation().X - bg.AbsolutePosition.X) / bg.AbsoluteSize.X
                x = math.clamp(x,0,1)
                fill.Size = UDim2.new(x,0,1,0)
                knob.Position = UDim2.new(x,-7,0.5,-7)
                local val = math.floor(min + (max-min)*x)
                label.Text = text .. ": " .. val
                callback(val)
            end)
            i.Changed:Connect(function()
                if i.UserInputState == Enum.UserInputState.End then conn:Disconnect() end
            end)
        end
    end)
end

local flyBtn = button("Toggle Fly",0.1)
local flying = false
local bv, bg
local flySpeed = 65

flyBtn.MouseButton1Click:Connect(function()
    flying = not flying
    if flying then
        bg = Instance.new("BodyGyro", root)
        bg.MaxTorque = Vector3.new(9e9,9e9,9e9)
        bg.P = 1e5

        bv = Instance.new("BodyVelocity", root)
        bv.MaxForce = Vector3.new(9e9,9e9,9e9)

        hum.PlatformStand = true
    else
        if bg then bg:Destroy() end
        if bv then bv:Destroy() end
        hum.PlatformStand = false
    end
end)

RS.RenderStepped:Connect(function()
    if flying and bv and bg then
        local cam = workspace.CurrentCamera
        bg.CFrame = cam.CFrame

        local dir = Vector3.zero
        if UIS:IsKeyDown(Enum.KeyCode.W) then dir += cam.CFrame.LookVector end
        if UIS:IsKeyDown(Enum.KeyCode.S) then dir -= cam.CFrame.LookVector end
        if UIS:IsKeyDown(Enum.KeyCode.A) then dir -= cam.CFrame.RightVector end
        if UIS:IsKeyDown(Enum.KeyCode.D) then dir += cam.CFrame.RightVector end
        if UIS:IsKeyDown(Enum.KeyCode.Space) then dir += Vector3.new(0,1,0) end
        if UIS:IsKeyDown(Enum.KeyCode.LeftControl) then dir -= Vector3.new(0,1,0) end

        if dir.Magnitude > 0 then
            dir = dir.Unit
        end

        bv.Velocity = dir * flySpeed
    end
end)

local noclipBtn = button("Toggle No Clip",0.19)
local noclip = false

noclipBtn.MouseButton1Click:Connect(function()
    noclip = not noclip
end)

RS.Stepped:Connect(function()
    if noclip and char then
        for _,v in pairs(char:GetDescendants()) do
            if v:IsA("BasePart") then
                v.CanCollide = false
            end
        end
    end
end)

local jumpBtn = button("Toggle Inf Jump",0.28)
local infJump = false
local holding = false
local plate

jumpBtn.MouseButton1Click:Connect(function()
    infJump = not infJump
end)

UIS.InputBegan:Connect(function(i)
    if infJump and i.KeyCode == Enum.KeyCode.Space then
        holding = true
        plate = Instance.new("Part", workspace)
        plate.Size = Vector3.new(5,0.2,5)
        plate.Anchored = true
        plate.CanCollide = true
        plate.Transparency = 0.6
        plate.Color = Color3.new(0,0,0)
    end
end)

UIS.InputEnded:Connect(function(i)
    if i.KeyCode == Enum.KeyCode.Space then
        holding = false
        if plate then plate:Destroy() plate = nil end
    end
end)

RS.Heartbeat:Connect(function()
    if infJump and holding and plate then
        plate.CFrame = root.CFrame * CFrame.new(0,-3,0)
    end
end)

local infHealthBtn = button("Toggle Infinite Health",0.37)
local infHealth = false

infHealthBtn.MouseButton1Click:Connect(function()
    infHealth = not infHealth
end)

RS.Heartbeat:Connect(function()
    if infHealth and hum then
        hum.Health = hum.MaxHealth
    end
end)

local TpBtn = button("TP forward", 0.55)
TpBtn.MouseButton1Click:Connect(function()
    local player = game:GetService("Players").LocalPlayer
    local char = player.Character
    if not char then return end

    local hrp = char:FindFirstChild("HumanoidRootPart")
    if not hrp then return end

    hrp.CFrame = hrp.CFrame + (hrp.CFrame.LookVector * 6)
end)

local Players = game:GetService("Players")
local RunService = game:GetService("RunService")
local LocalPlayer = Players.LocalPlayer
local Camera = workspace.CurrentCamera

local espBtn = button("Toggle ESP", 0.65)
local espEnabled = false
local espObjects = {}

local function createESP(char, name)
    if not char then return end
    local head = char:FindFirstChild("Head")
    local rootPart = char:FindFirstChild("HumanoidRootPart")
    if not head or not rootPart then return end

    local billboard = Instance.new("BillboardGui")
    billboard.Name = "ESP_Billboard"
    billboard.Adornee = head
    billboard.Size = UDim2.new(0, 100, 0, 30)
    billboard.StudsOffset = Vector3.new(0, 2, 0)
    billboard.AlwaysOnTop = true
    billboard.Parent = LocalPlayer:WaitForChild("PlayerGui")

    local nameLabel = Instance.new("TextLabel", billboard)
    nameLabel.Size = UDim2.new(1, 0, 1, 0)
    nameLabel.BackgroundTransparency = 1
    nameLabel.Text = name
    nameLabel.TextColor3 = Color3.new(1, 1, 1)
    nameLabel.TextStrokeTransparency = 0
    nameLabel.Font = Enum.Font.GothamBold
    nameLabel.TextScaled = true

    local box = Instance.new("BoxHandleAdornment")
    box.Adornee = rootPart
    box.Size = Vector3.new(2, 5, 1)
    box.Color3 = Color3.new(1, 1, 1)
    box.Transparency = 0.5
    box.AlwaysOnTop = true
    box.ZIndex = 0
    box.Parent = rootPart

    local tracerLine = Drawing.new("Line")
    tracerLine.Color = Color3.new(1, 1, 1)
    tracerLine.Thickness = 1.5
    tracerLine.Visible = false

    espObjects[char] = {
        billboard = billboard,
        box = box,
        tracer = tracerLine,
        char = char,
        rootPart = rootPart,
        head = head
    }
end

local function removeESP(char)
    local data = espObjects[char]
    if data then
        if data.billboard then data.billboard:Destroy() end
        if data.box then data.box:Destroy() end
        if data.tracer then data.tracer:Remove() end
        espObjects[char] = nil
    end
end

local function updateESP()
    local viewportSize = Camera.ViewportSize
    local bottomCenter = Vector2.new(viewportSize.X / 2, viewportSize.Y)

    for char, data in pairs(espObjects) do
        if char.Parent then
            local rootPos, onScreenRoot = Camera:WorldToViewportPoint(data.rootPart.Position)
            local headPos, onScreenHead = Camera:WorldToViewportPoint(data.head.Position + Vector3.new(0, 0.5, 0))
            data.billboard.Enabled = onScreenHead

            if onScreenRoot then
                data.tracer.From = bottomCenter
                data.tracer.To = Vector2.new(rootPos.X, rootPos.Y)
                data.tracer.Visible = true
            else
                data.tracer.Visible = false
            end
        else
            removeESP(char)
        end
    end
end

local function clearAllESP()
    for char, _ in pairs(espObjects) do
        removeESP(char)
    end
end

local function scanHumanoids()
    for _, obj in pairs(workspace:GetDescendants()) do
        if obj:IsA("Humanoid") and obj.Parent then
            local char = obj.Parent
            if not espObjects[char] then
                local name = char:FindFirstChild("Humanoid") and obj.Parent.Name or "NPC"
                createESP(char, name)
            end
        end
    end
end

espBtn.MouseButton1Click:Connect(function()
    espEnabled = not espEnabled

    if espEnabled then
        RunService.RenderStepped:Connect(function()
            if espEnabled then
                scanHumanoids()
                updateESP()
            end
        end)
    else
        clearAllESP()
    end
end)

local fpsBtn = button("FPS Boost", 0.75)

fpsBtn.MouseButton1Click:Connect(function()
    setfpscap(10)
    wait(0.2)
    setfpscap(144)
end)

local chatSpyBtn = button("Chat Spy", 0.85)

chatSpyBtn.MouseButton1Click:Connect(function()
    local l = game:GetService("TextChatService")
    l.ChatWindowConfiguration.Enabled = true
end)

labeledSlider("Walk Speed",0.5,16,120,16,function(v)
    hum.WalkSpeed = v
end)

UIS.InputBegan:Connect(function(i,gp)
    if not gp and i.KeyCode == Enum.KeyCode.L then
        frame.Visible = not frame.Visible
    end
end)

player.CharacterAdded:Connect(function(c)
    char = c
    hum = c:WaitForChild("Humanoid")
    root = c:WaitForChild("HumanoidRootPart")
end)
