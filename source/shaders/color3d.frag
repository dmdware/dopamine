""
"#version 120\r\n"
"varying vec3 ip;"
"void main()"
"{"
"	gl_FragColor = vec4(1,1,1, 1.0);//gl_FragColor = vec4(ip, 0.06*(1.0 - gl_FragCoord.z)*(1.0 - gl_FragCoord.z));//vec4(gl_FragCoord.xyz/2.0 + vec3(0.5, 0.5, 0.0), 0.5 + (0.5 - gl_FragCoord.z/2.0) );\r\n"
"}"
""