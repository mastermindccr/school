<?xml version='1.0' encoding='UTF-8'?>
<Project Type="Project" LVVersion="20008000">
	<Item Name="My Computer" Type="My Computer">
		<Property Name="server.app.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.control.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="server.tcp.enabled" Type="Bool">false</Property>
		<Property Name="server.tcp.port" Type="Int">0</Property>
		<Property Name="server.tcp.serviceName" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.tcp.serviceName.default" Type="Str">My Computer/VI Server</Property>
		<Property Name="server.vi.callsEnabled" Type="Bool">true</Property>
		<Property Name="server.vi.propertiesEnabled" Type="Bool">true</Property>
		<Property Name="specify.custom.address" Type="Bool">false</Property>
		<Item Name="SUB VI" Type="Folder">
			<Item Name="EqualStringProcess.vi" Type="VI" URL="../SUB VI/EqualStringProcess.vi"/>
			<Item Name="StringProcess.vi" Type="VI" URL="../SUB VI/StringProcess.vi"/>
		</Item>
		<Item Name="Type Define" Type="Folder">
			<Item Name="Data.ctl" Type="VI" URL="../Type Define/Data.ctl"/>
			<Item Name="Enum.ctl" Type="VI" URL="../Type Define/Enum.ctl"/>
			<Item Name="message.ctl" Type="VI" URL="../Type Define/message.ctl"/>
		</Item>
		<Item Name="Calc2.vi" Type="VI" URL="../Calc2.vi"/>
		<Item Name="Dependencies" Type="Dependencies">
			<Item Name="vi.lib" Type="Folder">
				<Item Name="NI_Gmath.lvlib" Type="Library" URL="/&lt;vilib&gt;/gmath/NI_Gmath.lvlib"/>
			</Item>
			<Item Name="lvanlys.dll" Type="Document" URL="/&lt;resource&gt;/lvanlys.dll"/>
		</Item>
		<Item Name="Build Specifications" Type="Build"/>
	</Item>
</Project>
