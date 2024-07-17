const express = require('express');
// MongoDB와 연결
const app = express();
const bodyParser = require('body-parser');

app.use(bodyParser.json());
// 로그인 요청을 처리하는 라우터
const { MongoClient } = require('mongodb');

var countID = 0;

app.post('/login', async (req, res) => {
    // 클라이언트로부터 전송된 요청 본문에서 이메일과 패스워드 추출
    const { email, pwd } = req.body;

    // MongoDB와 연결
    const client = new MongoClient('mongodb://localhost:27017');
    await client.connect();

    try {
        // 데이터베이스 선택
        const db = client.db('discord');

        // 사용자 컬렉션 선택
        const collection = db.collection('users');

        // 이메일을 기준으로 사용자 정보 조회
        const user = await collection.findOne({ email: email, pwd: pwd });

        if (user) {
            countID += 1;
            // 사용자가 존재할 때의 로직
            res.status(200).json({ id: user.userid, name: user.name});
        }
        else {
            // 사용자가 존재하지 않을 때의 로직
            res.status(404).json({ message: '사용자가 존재하지 않습니다.' });
        }
    }
    catch (error) {
        // 오류 발생 시
        console.error('데이터베이스 작업 중 오류 발생:', error);
        res.status(500).json({ message: '데이터베이스 작업 중 오류가 발생했습니다.' });
    }
    finally {
        // MongoDB 클라이언트 연결 종료
        await client.close();
    }
});

app.post('/signup', async (req, res) =>
{
    // 클라이언트로부터 전송된 요청 본문에서 이메일과 패스워드 추출
    const { email, pwd, name } = req.body;

    // MongoDB와 연결
    const client = new MongoClient('mongodb://localhost:27017');
    await client.connect();

    try
    {
        const db = client.db('discord');
        // 이메일을 기준으로 사용자 정보 조회
        const user = { email: email, pwd: pwd, name: name };
        const result = await db.collection('users').insertOne(user);
    }
    catch (error)
    {
        // 오류 발생 시
        console.error('데이터베이스 작업 중 오류 발생:', error.code);
        res.status(500).json({ message: '데이터베이스 작업 중 오류가 발생했습니다.' });
    }
    finally
    {
        // MongoDB 클라이언트 연결 종료
        res.status(200).json({ message: '사용자 생성 완료.' });
        await client.close();
    }
});

app.post('/messages', async (req, res) => {
    // 채널ID를 통해 최근 대화 내용 전송
    const { channel_id } = req.body;

    // MongoDB와 연결
    const client = new MongoClient('mongodb://localhost:27017');
    await client.connect();

    try {
        // 데이터베이스 선택
        const db = client.db('discord');

        // 사용자 컬렉션 선택
        const collection = db.collection('messages');

        // 이메일을 기준으로 사용자 정보 조회
        const user = { email: email, pwd: pwd, name: name, nickname: nickname };
        const result = await db.collection('users').insertOne(user);
    }
    catch (error) {
        // 오류 발생 시
        console.error('데이터베이스 작업 중 오류 발생:', error.code);
        res.status(500).json({ message: '데이터베이스 작업 중 오류가 발생했습니다.' });
    }
    finally {
        // MongoDB 클라이언트 연결 종료
        res.status(200).json({ message: '사용자 생성 완료.' });
        await client.close();
    }
});

// 서버 시작
const PORT = 3000;
app.listen(PORT, () => {
    console.log(`서버가 ${PORT} 포트에서 실행 중`);
});
