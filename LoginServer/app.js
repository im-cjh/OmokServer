const bcrypt = require('bcrypt');
const express = require('express');
const { MongoClient } = require('mongodb');
const bodyParser = require('body-parser');

const app = express();
app.use(bodyParser.json());

const client = new MongoClient('mongodb://localhost:27017');

app.post('/signup', async (req, res) => {
    const { email, pwd, name } = req.body;

    try {
        await client.connect();
        const db = client.db('discord');
        const userCollection = db.collection('users');
        const counterCollection = db.collection('counters');

        // 비밀번호 해싱
        const saltRounds = 10;
        const hashedPwd = await bcrypt.hash(pwd, saltRounds);

        // 사용자 ID 생성
        const counterDoc = await counterCollection.findOneAndUpdate(
            { _id: "userIdCounter" },
            { $inc: { sequence_value: 1 } },
            { returnDocument: "after", upsert: true }
        );

        //// 반환 값 확인
        //if (!counterDoc || !counterDoc.value) {
        //    throw new Error('Counter document not found or created');
        //}

        const userId = counterDoc.sequence_value;

        // 사용자 데이터 저장
        const user = { email, pwd: hashedPwd, name, userid: userId, win: 0, lose: 0 };
        await userCollection.insertOne(user);

        res.status(200).json({ message: '사용자 생성 완료.', userId: userId });
    } catch (error) {
        console.error('데이터베이스 작업 중 오류 발생:', error);
        res.status(500).json({ message: '데이터베이스 작업 중 오류가 발생했습니다.' });
    } finally {
        await client.close();
    }
});


app.post('/login', async (req, res) => {
    const { email, pwd } = req.body;

    try {
        await client.connect();
        const db = client.db('discord');
        const collection = db.collection('users');

        const user = await collection.findOne({ email });
        if (user && await bcrypt.compare(pwd, user.pwd)) {
            res.status(200).json({ id: user.userid, name: user.name, win: user.win, lose: user.lose });
        } else {
            res.status(404).json({ message: '사용자가 존재하지 않거나 비밀번호가 일치하지 않습니다.' });
        }
    } catch (error) {
        console.error('데이터베이스 작업 중 오류 발생:', error);
        res.status(500).json({ message: '데이터베이스 작업 중 오류가 발생했습니다.' });
    } finally {
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

app.post('/update_win_rate', async (req, res) => {
    const { userid, flag } = req.body; // req.body에서 userid와 flag 추출

    // MongoDB와 연결
    const client = new MongoClient('mongodb://localhost:27017');
    await client.connect();

    try {
        // 데이터베이스 선택
        const db = client.db('discord');

        // 사용자 컬렉션 선택
        const collection = db.collection('users');

        // 업데이트할 필드 결정
        const updateField = flag ? { win: 1 } : { lose: 1 };

        // 사용자 정보 업데이트
        const result = await collection.updateOne(
            { userid: userid }, // 업데이트할 문서 필터
            { $inc: updateField } // win 또는 lose 값을 1 증가
        );

        if (result.matchedCount === 0) {
            // 일치하는 사용자가 없을 경우
            res.status(404).json({ message: '사용자를 찾을 수 없습니다.' });
        } else {
            // 업데이트 성공
            res.status(200).json({ message: '사용자 정보가 성공적으로 업데이트되었습니다.' });
        }
    } catch (error) {
        // 오류 발생 시
        console.error('데이터베이스 작업 중 오류 발생:', error);
        res.status(500).json({ message: '데이터베이스 작업 중 오류가 발생했습니다.' });
    } finally {
        // MongoDB 클라이언트 연결 종료
        await client.close();
    }
});


// 서버 시작
const PORT = 3000;
app.listen(PORT, () => {
    console.log(`서버가 ${PORT} 포트에서 실행 중`);
});
