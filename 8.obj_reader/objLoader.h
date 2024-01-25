#pragma once



//bool Object::readObjFIle(char* pFileName)
//{
//    char text[256], text1[256];
//
//    int faceVertex[15], UV, nomal;
//    char Num;
//
//    vector<string>line;
//
//    ifstream pFile(pFileName);
//
//    if (!pFile)
//        return false;
//
//    while (!pFile.eof())
//    {
//        pFile.getline(text, 256);
//        line.push_back(text);
//    }
//
//    for (int i = 0; i < line.size(); i++)
//    {
//        if (line[i][0] == 'v' && line[i][1] == ' ') {
//            sscanf_s(line[i].c_str(), "v %f %f %f", &vertex[nNumVertex][0], &vertex[nNumVertex][1], &vertex[nNumVertex][2]);
//            nNumVertex++;
//        }
//
//        else if (line[i][0] == 'v' && line[i][1] == 't') {
//            sscanf_s(line[i].c_str(), "vt %f %f", &uv[nNumUV][0], &uv[nNumUV][1]);
//            nNumUV++;
//            bHasUV = true;
//
//        }
//        else if (line[i][0] == 'v' && line[i][1] == 'n') {
//            sscanf_s(line[i].c_str(), "vn %f %f %f", &vertexNormal[nNumNormal][0], &vertexNormal[nNumNormal][1], &vertexNormal[nNumNormal][2]);
//            nNumNormal++;
//            bHasNormal = true;
//        }
//
//        else if (i < line.size() && line[i][0] == 'f') {
//
//            int j = 0, numVertex = 0;
//
//            for (; j < line[i].size(); j++) {
//                if (line[i][j] == ' ') {
//                    numVertex++;
//                }
//            }
//
//            if (line[i][j - 1] == ' ') {
//                numVertex--;
//            }
//
//            face[nNumFace].m_nNumVertex = numVertex;
//            int faceVerNum = 0, faceVerNorNum = 0, faceUVNum = 0;
//
//            if (line[i][1] == ' ' && bHasUV && bHasNormal) {
//
//                for (int j = 1; j < line[i].size(); j++) {
//                    if (line[i][j] == ' ') {
//                        sscanf_s(&line[i][j + 1], "%d/%d/%d", &faceVertex[faceVerNum], &UV, &nomal);
//                        faceVerNum++;
//                    }
//                }
//                for (int j = 0; j < face[nNumFace].m_nNumVertex; j++) {
//                    face[nNumFace].m_vertex[j] = faceVertex[j];
//                }
//            }
//
//            else if (line[i][1] == ' ' && !bHasUV && bHasNormal) {
//                for (int j = 1; j < line[i].size(); j++) {
//                    if (line[i][j] == ' ') {
//                        sscanf_s(&line[i][j + 1], "%d//%d", &faceVertex[faceVerNum], &nomal);
//                        faceVerNum++;
//                    }
//                }
//                for (int j = 0; j < face[nNumFace].m_nNumVertex; j++) {
//                    face[nNumFace].m_vertex[j] = faceVertex[j];
//                }
//            }
//
//            else {
//                for (int j = 1; j < line[i].size(); j++) {
//                    if (line[i][j] == ' ') {
//                        sscanf_s(&line[i][j + 1], "%d", &faceVertex[faceVerNum]);
//                        faceVerNum++;
//                    }
//                }
//                for (int j = 0; j < face[nNumFace].m_nNumVertex; j++) {
//                    face[nNumFace].m_vertex[j] = faceVertex[j];
//                }
//            }
//
//
//
//            nNumFace++;
//        }
//    }
//    pFile.close();
//
//    return true;
//}

















//else if (line[i][0] == 'f' && line[i][1] == ' ')
//{
//	if (tryOnce)
//	{
//		for (int j = 0; j < line[i].size(); j++)
//		{
//			if (line[i].c_str(), line[i][j] == '/')
//			{
//				isDashExist = true;
//				break;
//			}
//		}
//		tryOnce = false;
//	}


//	if (isDashExist == true)
//	{
//		int a, b, c; //to store mesh index
//		int A, B, C; //to store texture index
//		//std::istringstream v;
//	    //v.str(line.substr(2));
//		const char* chh = line[i].c_str();
//		sscanf(chh, "f %d//%f %d//%f %d//%f ", &m_face[m_nNumFace].m_vertex[0], &fake_faceNormal[m_nNumFace][0], &m_face[m_nNumFace].m_vertex[1], &fake_faceNormal[m_nNumFace][1], &m_face[m_nNumFace].m_vertex[2], &fake_faceNormal[m_nNumFace][2]);



//		for (int i = 0; i < 10; i++)
//		{

//			if (m_face[m_nNumFace].m_vertex[i] != 0)
//			{
//				m_face[m_nNumFace].m_nNumVertex++;
//			}
//		}

//		m_nNumFace++;

//		for (int i = 0; i < m_face[m_nNumFace].m_nNumVertex - 3; i++)
//		{
//			m_face[m_nNumFace].m_vertex[0] = m_face[m_nNumFace - i - 1].m_vertex[0];
//			m_face[m_nNumFace].m_vertex[1] = m_face[m_nNumFace - i - 1].m_vertex[2 + i];
//			m_face[m_nNumFace].m_vertex[2] = m_face[m_nNumFace - i - 1].m_vertex[3 + i];

//			m_nNumFace++;
//		}
//		i++;
//	}


//	else
//	{
//		sscanf_s(&line[i][2], "%d %d %d %d %d %d %d", &m_face[m_nNumFace].m_vertex[0], &m_face[m_nNumFace].m_vertex[1], &m_face[m_nNumFace].m_vertex[2], &m_face[m_nNumFace].m_vertex[3], &m_face[m_nNumFace].m_vertex[3], &m_face[m_nNumFace].m_vertex[4], &m_face[m_nNumFace].m_vertex[5], &m_face[m_nNumFace].m_vertex[6], &m_face[m_nNumFace].m_vertex[7]);

//		for (int i = 0; i < 10; i++)
//		{
//			
//			if (m_face[m_nNumFace].m_vertex[i] != 0)
//			{
//				m_face[m_nNumFace].m_nNumVertex++;
//			}
//		}

//		m_nNumFace++;

//		for (int i = 0; i < m_face[m_nNumFace].m_nNumVertex-3; i++)
//		{
//			m_face[m_nNumFace].m_vertex[0] = m_face[m_nNumFace - i - 1].m_vertex[0];
//			m_face[m_nNumFace].m_vertex[1] = m_face[m_nNumFace - i - 1].m_vertex[2 + i];
//			m_face[m_nNumFace].m_vertex[2] = m_face[m_nNumFace - i - 1].m_vertex[3 + i];

//			m_nNumFace++;
//		}

//		i++;

//	}
//}
