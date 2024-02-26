varying vec2 v_uv;
uniform sampler2D u_fruites;
uniform float time;
uniform float option;
uniform float ex;

void main()
{
    //----------------------EXERCICI 1-----------------------------
    
    //definim el color fora de l'if ja que sinó ens salta error al gl_FragColor
    vec3 color;
    vec2 uv_p;
    vec4 texture_color;
    vec3 vermell= vec3(1,0,0);
    vec3 blau= vec3(0,0,1);
    vec3 verd= vec3(0,1,0);
    vec3 color2;
    
    if (ex==1.0){
        //  OPCIÓ A) mix entre blau i vermell horitzontal
        if (option==1.0){
            
            color= mix(blau, vermell, v_uv.x);
          
            
        //  OPCIÓ B) punt gris mitg pantall
        } else if (option==2.0){
            
            //centre de la pantalla
            vec2 centre= vec2(0.5,0.5);
            //distancia del centre de la pantalla a les V-uv
            color = vec3(distance(centre, v_uv));

           
        //  OPCIÓ C) linies vermelles verticals creuades amb linies blaves horitzontals
        } else if (option==3.0){
            
        // pertant fem el color: (0,0,1) * una funció (intermitent, conte el 0 i el 1) amb un pas ( quan de rapid volem que iteri aquesta funcio ciclica entre -1,0,1 (pas gran-> iteració més ràpida -> més linies, això mult per l'eix sobre el que volem interar v_uv.y) ( amb el 0 )
            color = vermell*sin(v_uv.y*30.0)+blau*sin(v_uv.x*40.0) ;
        
            
        //  OPCIÓ D) quadricula amb gradient verd i vemrell diagonal
        } else if(option==4.0){
            // posicio 0,0 tenim negre ja que cada compoenent queda 0 per la funció floor
            // posicio 1,1 tenim groc ja que cada compoenent queda 1 per la funció floor i pertant és fa el mix dels dos colors
            //el pas que nosaltres entrem dins del floor fa aquest efecte dels quadrats tant x la component x com per la y ja que escala aquell floor que nomes estaria entre 0 i 1 a 0 i 20 (l'hem de tornar a dividir entre 20/10 respectivament per normalitzarho i que el pes del color no es dispari
            
            color =(floor(v_uv.x*20.0)/20.0)*vermell +(floor(v_uv.y*10.0)/10.0)*verd;
            
         
        //  OPCIÓ E) taulell blanc i negre
        }else if(option==5.0){

            vec3 color1 = vec3(256,256,256)*floor(sin(v_uv.y*30.0))*floor(sin(v_uv.x*50.0)) ;
            vec3 color2 = vec3(256,256,256)*floor(sin(v_uv.y*-30.0))*floor(sin(v_uv.x*-50.0)) ;
            color= color1+color2;
            
        }
        // OPCIÓ F) funció sinosoidal
        else if (option==6.0){
            float A=0.25;
            float despl=0.5;
            //funcio sinosidal
            float borde= A*sin(v_uv.x*6.5)+despl;
            
            //Pinta sobre sin
            vec3 color1 = ((1.0-v_uv.y)*verd*step(borde, v_uv.y));
            //l'step dona 0 si esta per sota del borde(edge) i 1 per sobre
            //negre -> V_uv = 1 ( dalt de la pantalla)
            //verd -> V_uv=0 (baix pantalla -> no pinta )
            
            // pinta la part de sota
            vec3 color2 = ((v_uv.y)*verd*(1.0-step(borde, v_uv.y)));
            //l'step dona 0 si esta per sobre del borde(edge) i 1 per sota
            //verd -> V_uv = 1 ( dalt de la pantalla-> no pinta )
            //negre -> V_uv=0 (baix pantalla )
            
            color = color1 + color2;

        }
        gl_FragColor = vec4(color, 1.0);
    
   

        
//    //---------------------------EXERCICI 2---------------------------------
    }else if (ex==2.0){
        

        // OPCIÓ A) BN
        if(option==1.0){
            texture_color=texture2D(u_fruites,v_uv);
            texture_color=vec4 ((texture_color.x+texture_color.y+texture_color.z)/3.0);


        // OPCIÓ B)invers
        }else if(option==2.0){
            texture_color=texture2D(u_fruites,v_uv);
            texture_color= vec4(1)-texture_color;
            
            
        // OPCIÓ C) groc
        }else if(option==3.0){
            texture_color=texture2D(u_fruites,v_uv);
            // normalitzem les coordenades x i y per obtenir sempre la seva barreja (groc)
            vec2 sum = vec2((texture_color.x+texture_color.y)/2.0);
            // anul·lem la coordenada z (blau) ja que no ens interessa per formar aquest color
            texture_color = vec4(sum,0,texture_color.w);

            
            
        // OPCIÓ D) BN Step
        }else if(option==4.0){
            texture_color=texture2D(u_fruites,v_uv);
            
            //convertim l'escena en balnc i negre
            float sum = ( texture_color.x + texture_color.y + texture_color.z)/3.0;
            
            //eliminem els grisos de la imatge
            texture_color.xyz =
            texture_color.xyz*vec3(step(0.5 , sum)) + (1.0-texture_color.xyz)*vec3(step(0.5 , sum));
            
            
            
        // OPCIÓ E) fosc desde el centre
        }else if(option==5.0){
            texture_color=texture2D(u_fruites,v_uv);
            
            //centre de la pantalla
            vec2 centre= vec2(0.5,0.5);
            //distancia del centre de la pantalla a les V-uv
            float distancia= distance(centre,v_uv);
            //  Tenim texture_color i volem enfosquir les coordenades d'aquest vector quan es troben més allunyats del centre ( és a dir distance=1). Sabent que (0,0,0) és el negre, coordenada més fosca fem 1-distancia ja que quan la distancia sigui gran necesitem multiplicar el color per un numero petit.
            texture_color = texture_color*(1.0-distancia);

            
        
        // OPCIÓ F) BLUR
        }else if(option==6.0){

            //definim variables on guardem colors dels pixels veins
            vec4 texture_color1;
            vec4 texture_color2;
            vec4 texture_color3;
            vec4 texture_color4;
            vec4 texture_color5;
            vec4 texture_color6;
            vec4 texture_color7;
            vec4 texture_color8;

            //calculem i guardem els colors dels pixels veins
            texture_color=texture2D(u_fruites, v_uv);
            texture_color1=texture2D(u_fruites, vec2(v_uv.x+0.005,v_uv.y));
            texture_color2=texture2D(u_fruites, vec2(v_uv.x+0.005,v_uv.y+0.005));
            texture_color3=texture2D(u_fruites, vec2(v_uv.x-0.005,v_uv.y-0.005));
            texture_color4=texture2D(u_fruites, vec2(v_uv.x-0.005,v_uv.y));
            texture_color5=texture2D(u_fruites, vec2(v_uv.x,v_uv.y-0.005));
            texture_color6=texture2D(u_fruites, vec2(v_uv.x,v_uv.y+0.005));
            texture_color7=texture2D(u_fruites, vec2(v_uv.x-0.005,v_uv.y+0.005));
            texture_color8=texture2D(u_fruites, vec2(v_uv.x+0.005,v_uv.y-0.005));

            // sumem tots els colors a una mateixa variable
            texture_color= texture_color+texture_color1;
            texture_color= texture_color+texture_color2;
            texture_color= texture_color+texture_color3;
            texture_color= texture_color+texture_color4;
            texture_color= texture_color+texture_color5;
            texture_color= texture_color+texture_color6;
            texture_color= texture_color+texture_color7;
            texture_color= texture_color+texture_color8;
            
            //normalitzem la variable
            texture_color= texture_color/vec4(9.0);
            //tenint en compte no modificar la coordenada w
            texture_color= vec4(texture_color.xyz,1.0);

        }


        gl_FragColor = texture_color;
    
        //    //---------------------------EXERCICI 3---------------------------------
   
    } else if(ex==3.0){
        
        //  OPCIÓ A) animació 1
        if (option==1.0){
            // passem v_uv en base del temps (Variable) igual que en les quadricules
            //+ pas = + particions en pixel per espai establert
            // temps petit fara que nomes agafem els colors al voltant dels 0,0.
            // temps més gran fara que agafem els colors de tota la pantalla fent moltissimes particions ( molts pixels) per a cada V_uv. menys globalització d'un mateix color entre els pixels veins.
            uv_p = vec2(floor(v_uv.x*time)/time, floor(v_uv.y*time)/time); //funció no continua
            
            // farem el texture_color nomes de la zona que assignem a uv_p
            texture_color = texture2D(u_fruites, uv_p);
            gl_FragColor = texture_color;

        }else if(option==2.0){
            // encompte de passar el v_uv real anem passant la v_uv en base del temps (Variable)però  ja no com a quadricules, sinó amb una funció continua i periodica com el sinus.
        
            //sin=0 , veiem per pantalla el color de V_uv )(0,0)
            //sin augmenta, el nostre rang V-uv augmenta tambe.
            //posem abs per evitar sin - ( els quals ens tindrien aturats a l'esq baix de la pantalla)
            uv_p =abs(sin(time))*(v_uv); //funcio continua
            // farem el texture_color nomes de la zona que assignem a uv_p (condicionada pel sin).
            vec4 texture_color = texture2D(u_fruites, uv_p);
            gl_FragColor = texture_color;
        }else if (option==3.0){
            uv_p=v_uv;
            vec4 texture_color = texture2D(u_fruites, vec2(1.0-uv_p.x,uv_p.y));
        }
    }
    


}
